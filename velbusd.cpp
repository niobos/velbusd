#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <getopt.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sysexits.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdexcept>
#include <boost/ptr_container/ptr_list.hpp>
#include <ev.h>
#include <memory>

#include "Socket.hpp"
#include "TimestampLog.hpp"
#include "VelbusMessage/VelbusMessage.hpp"

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

struct connection {
	Socket sock;
	VelbusMessage::Deframer buf;
	std::string id;
	ev_io watcher;
};

std::auto_ptr<std::ostream> log;
Socket s_listen;
struct connection c_serial;
boost::ptr_list< struct connection > c_network;

std::string read(int from) throw(IOError, EOFreached) {
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

void write(int to, std::string const &what) throw(IOError) {
	int rv = write(to, what.data(), what.length());
	if( rv == -1 ) {
		char error_descr[256];
		strerror_r(errno, error_descr, sizeof(error_descr));
		std::string e;
		e = "Could not read(): ";
		e.append(error_descr);
		throw IOError( e );
	} else if( rv != what.length() ) {
		throw IOError( "Not enough bytes written" );
	}
}

void kill_connection(EV_P_ ev_io *w) {
	// Remove from event loop
	ev_io_stop(EV_A_ w );
	// Find and erase this connection in the list
	for( typeof(c_network.begin()) i = c_network.begin(); i != c_network.end(); ++i ) {
		if( &(i->watcher) == w ) {
			c_network.erase(i);
			break; // Stop searching
		}
	}
}

void serial_ready_to_read(EV_P_ ev_io *w, int revents) throw() {
	std::string buf;
	try {
		buf = read(c_serial.sock);
	} catch( IOError &e ) {
		*log << c_serial.id << " : IO error: " << e.what() << "\n" << std::flush;
		throw;
	} catch( EOFreached &e ) {
		*log << c_serial.id << " : EOF reached\n" << std::flush;
		throw;
	}

	c_serial.buf.add_data(buf);
	while(1) {
		std::auto_ptr<VelbusMessage::VelbusMessage> m = c_serial.buf.get_message();
		if( m.get() == NULL ) break;
		*log << c_serial.id << " : " << m->string() << "\n" << std::flush;

		for( typeof(c_network.begin()) i = c_network.begin(); i != c_network.end(); ++i ) {
			try {
				write(i->sock, m->message());
			} catch( IOError &e ) {
				*log << i->id << " : IO error, closing connection: " << e.what() << "\n" << std::flush;
				kill_connection(EV_A_ w);
			}
		}
	}
}

void socket_ready_to_read(EV_P_ ev_io *w, int revents) throw() {
	struct connection *c = reinterpret_cast<struct connection*>(w->data);
	std::string buf;
	try {
		buf = read(c->sock);

	} catch( IOError &e ) {
		*log << c->id << " : IO error, closing connection: " << e.what() << "\n" << std::flush;
		kill_connection(EV_A_ w);
		return; // early

	} catch( EOFreached &e ) {
		*log << c->id << " : Disconnect\n" << std::flush;
		kill_connection(EV_A_ w);
		return; // early
	}

	c->buf.add_data(buf);
	while(1) {
		std::auto_ptr<VelbusMessage::VelbusMessage> m = c->buf.get_message();
		if( m.get() == NULL ) break;
		*log << c->id << " : " << m->string() << "\n" << std::flush;

		try {
			write(c_serial.sock, m->message());
		} catch( IOError &e ) {
			throw;
		}
		for( typeof(c_network.begin()) i = c_network.begin(); i != c_network.end(); ++i ) {
			if( &(*i) == c ) continue; // Don't loop input to same socket
			try {
				write(i->sock, m->message());
			} catch( IOError &e ) {
				*log << i->id << " : IO error, closing connection: " << e.what() << "\n" << std::flush;
				kill_connection(EV_A_ w);
			}
		}
	}
}

void incomming_connection(EV_P_ ev_io *w, int revents) {
	std::auto_ptr<struct connection> new_con( new struct connection );
	std::auto_ptr<SockAddr::SockAddr> client_addr;
	new_con->sock = s_listen.accept(&client_addr);
	new_con->id = client_addr->string();
	*log << new_con->id << " : Connection opened\n" << std::flush;

	ev_io_init( &new_con->watcher, socket_ready_to_read, new_con->sock, EV_READ );
	new_con->watcher.data = new_con.get();
	ev_io_start( EV_A_ &new_con->watcher );

	c_network.push_back( new_con.release() );
}

int main(int argc, char* argv[]) {
	// Defaults
	std::string serial_port("/dev/ttyS0");
	std::string bind_addr("[::1]:8445");

	log.reset( new TimestampLog( std::cerr ) );

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

	{ // Open serial port
		c_serial.id = "SERIAL";
		c_serial.sock = open(serial_port.c_str(), O_RDWR | O_NOCTTY);
		// Open in Read-Write; don't become controlling TTY
		if( c_serial.sock == -1 ) {
			fprintf(stderr, "Could not open \"%s\": ", serial_port.c_str());
			perror("open()");
			exit(EX_NOINPUT);
		}
		*log << "Opened port \"" << serial_port << "\"\n" << std::flush;

		// Setting up port
		struct termios options;
		tcgetattr(c_serial.sock, &options);

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
		tcsetattr(c_serial.sock, TCSANOW, &options);

		// Manually setting RTS high & DTR low
		int status;
		ioctl(c_serial.sock, TIOCMGET, &status); // Get MODEM-bits
		status &= ~TIOCM_DTR; // DTR = 0
		status |= TIOCM_RTS; // RTS = 1
		ioctl(c_serial.sock, TIOCMSET, &status); // Write MODEM-bits

		*log << "Configured port \"" << serial_port << "\"\n" << std::flush;
	}

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
		s_listen = Socket::socket( (*bind_sa)[0].proto_family() , SOCK_STREAM, 0);
		s_listen.set_reuseaddr();
		s_listen.bind((*bind_sa)[0]);
		s_listen.listen(MAX_CONN_BACKLOG);
		*log << "Listening on " << (*bind_sa)[0].string() << "\n" << std::flush;
	}

	{
		ev_io_init( &c_serial.watcher, serial_ready_to_read, c_serial.sock, EV_READ );
		ev_io_start( EV_DEFAULT_ &c_serial.watcher );

		ev_io e_listen;
		ev_io_init( &e_listen, incomming_connection, s_listen, EV_READ );
		ev_io_start( EV_DEFAULT_ &e_listen );

		*log << "Setup done, starting event loop\n" << std::flush;

		ev_loop(EV_DEFAULT_ 0);
	}

	return 0;
}
