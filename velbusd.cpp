#include <stdio.h>
#include <string>
#include <sstream>
#include <getopt.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sysexits.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include <stdexcept>
#include <boost/ptr_container/ptr_list.hpp>
#include <syslog.h>

#include "Socket.hpp"

static const size_t READ_SIZE = 4096;
static const int MAX_CONN_BACKLOG = 32;

class IOError : public std::runtime_error {
public:
	IOError( const std::string &what ) :
	    std::runtime_error( what ) {}
};
class EOFreached : public std::runtime_error {
public:
	EOFreached() :
	    std::runtime_error( std::string("EOF reached") ) {}
};

struct client {
	Socket sock;
	std::string id;
};

std::string read(int from) {
	char buf[READ_SIZE];
	int n = read(from, buf, sizeof(buf));
	if( n == -1 ) {
		char error_descr[256];
		strerror_r(errno, error_descr, sizeof(error_descr));
		std::string e;
		e = "Could not read(): ";
		e.append(error_descr);
		throw IOError( e );
	} else if( n == 0 ) {
		throw EOFreached();
	}
	return std::string(buf, n);
}

void write(int to, std::string const &what) {
	int rv = write(to, what.data(), what.length());
	if( rv == -1 ) {
		char error_descr[256];
		strerror_r(errno, error_descr, sizeof(error_descr));
		std::string e;
		e = "Could not write(): ";
		e.append(error_descr);
		throw IOError( e );
	} else if( rv != what.length() ) {
		throw IOError( std::string("write() wrote incorrect number of bytes") );
	}
}

int main(int argc, char* argv[]) {
	// Defaults
	std::string serial_port("/dev/ttyS0");
	std::string bind_addr("[::1]:8445");

	{ // Parse options
		char optstring[] = "?hs:b:";
		struct option longopts[] = {
			{"help",			no_argument, NULL, '?'},
			{"serialport",		required_argument, NULL, 's'},
			{"bind",			required_argument, NULL, 'b'},
			{NULL, 0, 0, 0}
		};
		int longindex;
		int opt;
		while( (opt = getopt_long(argc, argv, optstring, longopts, &longindex)) != -1 ) {
			switch(opt) {
			case '?':
			case 'h':
				fprintf(stderr,
				//  >---------------------- Standard terminal width ---------------------------------<
					"Options:\n"
					"  -h -? --help                    Displays this help message and exits\n"
					"  --serialport -s  /dev/ttyS0     The serial device to use\n"
					"  --bind -b host:port             Bind to the specified address\n"
					"                                  host and port resolving can be bypassed by\n"
					"                                  placing [] around them\n"
					);
				exit(EX_USAGE);
			case 's':
				serial_port = optarg;
				break;
			case 'b':
				bind_addr = optarg;
				break;
			}
		}
	}

	Socket fd_ser; // Not really a socket, but needs close() at the end as well
	{ // Open serial port
		fd_ser = open(serial_port.c_str(), O_RDWR | O_NOCTTY);
		// Open in Read-Write; don't become controlling TTY
		if( fd_ser == -1 ) {
			fprintf(stderr, "Could not open \"%s\": ", serial_port.c_str());
			perror("open()");
			exit(EX_NOINPUT);
		}
		fprintf(stderr, "Opened \"%s\"\n", serial_port.c_str());

		// Setting up port
		struct termios options;
		tcgetattr(fd_ser, &options);

		cfsetispeed(&options, B38400);
		cfsetospeed(&options, B38400);

		options.c_cflag |= CLOCAL; // Don't change owner of port
		options.c_cflag |= CREAD; // Enable receiver

		options.c_cflag &= ~CSIZE; // Mask the character size bits
		options.c_cflag |= CS8;    // Select 8 data bits

		options.c_cflag &= ~PARENB; // Clear parity
		options.c_cflag &= ~CSTOPB; // Clear "2 Stopbits" => 1 stopbit

		options.c_cflag &= ~CRTSCTS; // Disable hardware flow control

		options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw mode

		options.c_iflag |= IGNPAR; // Ignore parity (since none is used)
		options.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable software flow control

		options.c_oflag &= ~OPOST; // Disable output processing = raw mode

		// Apply options
		tcsetattr(fd_ser, TCSANOW, &options);

		// Manually setting RTS high & DTR low
		int status;
		ioctl(fd_ser, TIOCMGET, &status); // Get MODEM-bits
		status &= ~TIOCM_DTR; // DTR = 0
		status |= TIOCM_RTS; // RTS = 1
		ioctl(fd_ser, TIOCMSET, &status); // Write MODEM-bits

		fprintf(stderr, "Configured \"%s\"\n", serial_port.c_str());
	}

	Socket sock_listen;
	{ // Open listening socket
		std::string host, port;

		/* Address format is
		 *   - hostname:portname
		 *   - [numeric ip]:portname
		 *   - hostname:[portnumber]
		 *   - [numeric ip:[portnumber]
		 */
		size_t c = bind_addr.rfind(":");
		if( c == std::string::npos ) {
			fprintf(stderr, "Invalid bind string \"%s\": could not find ':'\n", bind_addr.c_str());
			exit(EX_DATAERR);
		}
		host = bind_addr.substr(0, c);
		port = bind_addr.substr(c+1);

		std::auto_ptr< boost::ptr_vector< SockAddr::SockAddr> > bind_sa
			= SockAddr::resolve( host, port, 0, SOCK_STREAM, 0);
		if( bind_sa->size() == 0 ) {
			fprintf(stderr, "Can not bind to \"%s\": Could not resolve\n", bind_addr.c_str());
			exit(EX_DATAERR);
		} else if( bind_sa->size() > 1 ) {
			// TODO: allow this
			fprintf(stderr, "Can not bind to \"%s\": Resolves to multiple entries:\n", bind_addr.c_str());
			for( typeof(bind_sa->begin()) i = bind_sa->begin(); i != bind_sa->end(); i++ ) {
				fprintf(stderr, "  %s\n", i->string().c_str());
			}
			exit(EX_DATAERR);
		}
		sock_listen = Socket::socket( (*bind_sa)[0].proto_family() , SOCK_STREAM, 0);
		sock_listen.set_reuseaddr();
		sock_listen.bind((*bind_sa)[0]);
		sock_listen.listen(MAX_CONN_BACKLOG);
		fprintf(stderr, "Listening on %s\n", (*bind_sa)[0].string().c_str());
	}

	try { // Select loop
		fd_set rfds, rfds_;
		int fd_max = 0;
		FD_ZERO(&rfds_);
		FD_SET(fd_ser, &rfds_); if( fd_ser > fd_max ) fd_max = fd_ser;
		FD_SET(sock_listen, &rfds_); if( sock_listen > fd_max ) fd_max = sock_listen;

		boost::ptr_list< struct client > sock_clients;

		int rv;
		bool recalculate_select = 0;
		while( rfds = rfds_, rv = select( fd_max+1, &rfds, NULL, NULL, NULL) ) {
			if( rv == -1 ) {
				perror("select()");
				exit(EX_IOERR);
			} else if( rv == 0 ) {
				fprintf(stderr, "Weird: select() returned 0. bailing out");
				exit(EX_IOERR);
			} else {

				if( FD_ISSET(sock_listen, &rfds) ) {
					// New client connecting
					std::auto_ptr< struct client > c( new struct client );
					std::auto_ptr< SockAddr::SockAddr > client_addr;
					c->sock = sock_listen.accept(&client_addr);
					c->id = client_addr->string();

					fprintf(stderr, "Connect from %s\n", client_addr->string().c_str());

					FD_SET(c->sock, &rfds_); if( c->sock > fd_max ) fd_max = c->sock; // Add to select
					sock_clients.push_back( c.release() ); // Keep the client
				}

				if( FD_ISSET(fd_ser, &rfds) ) {
					std::string buf;
					try { buf = read(fd_ser); }
					catch( EOFreached &e ) {
						fprintf(stderr, "EOF reached on serial, bailing out...\n");
						exit(EX_OSERR);
					}
					for( typeof(sock_clients.begin()) i = sock_clients.begin(); i != sock_clients.end(); ++i) {
						write(i->sock, buf);
					}
				}

				for( typeof(sock_clients.begin()) i = sock_clients.begin(); i != sock_clients.end(); ++i) {
					if( FD_ISSET(i->sock, &rfds) ) {
						std::string buf;
						try { buf = read(i->sock); }
						catch( EOFreached &e ) {
							// EOF reached on socket, remove it from the list
							fprintf(stderr, "Disconnect from %s\n", i->id.c_str());
							typeof(i) temp = i; i--;
							sock_clients.erase(temp);
							recalculate_select = 1;
						}
						write(fd_ser, buf);
						for( typeof(sock_clients.begin()) j = sock_clients.begin(); j != sock_clients.end(); ++j) {
							if( j->sock == i->sock ) continue; // Don't echo back
							write(j->sock, buf);
						}
					}
				}

			}

			if( recalculate_select ) {
				fd_max = 0;
				FD_ZERO(&rfds_);
				FD_SET(fd_ser, &rfds_); if( fd_ser > fd_max ) fd_max = fd_ser;
				FD_SET(sock_listen, &rfds_); if( sock_listen > fd_max ) fd_max = sock_listen;
				for( typeof(sock_clients.begin()) i = sock_clients.begin(); i != sock_clients.end(); ++i) {
					FD_SET(i->sock, &rfds_); if( i->sock > fd_max ) fd_max = i->sock;
				}
				recalculate_select = 0;
			}
		}
	} catch ( IOError &e ) {
		fprintf(stderr, "IO error: %s\n", e.what() );
		exit(EX_IOERR);
	}

	return 0;
}
