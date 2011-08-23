#include <string>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <sysexits.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <stdio.h>
#include <stdexcept>
#include <boost/ptr_container/ptr_list.hpp>
#include <ev.h>
#include <memory>
#include <typeinfo>
#include <assert.h>

#include "Socket.hpp"
#include "utils/TimestampLog.hpp"
#include "utils/output.hpp"
#include "VelbusMessage/VelbusMessage.hpp"
#include "VelbusMessage/IntStatusRequest.hpp"
#include "VelbusMessage/BusOff.hpp"
#include "VelbusMessage/BusActive.hpp"
#include "VelbusMessage/RxBuffFull.hpp"
#include "VelbusMessage/RxReady.hpp"

static const size_t READ_SIZE = 4096;
static const int MAX_CONN_BACKLOG = 32;

class IOError : public std::runtime_error {
public:
	IOError( const std::string &what ) :
	    std::runtime_error( what ) {}
};
class WouldBlock : public IOError {
public:
	WouldBlock() :
	    IOError("Would block") {}
};
class EOFreached : public std::runtime_error {
public:
	EOFreached() :
	    std::runtime_error( std::string("EOF reached") ) {}
};

struct connection {
	Socket sock;
	std::string buf;
	std::string id;
	ev_io watcher;
};

std::auto_ptr<std::ostream> log;
Socket s_listen;
struct {
	struct connection conn;
	bool bus_active;
	bool rx_ready;
	} serial;
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
		if( errno == EAGAIN ) {
			throw WouldBlock();
		} // else

		char error_descr[256];
		strerror_r(errno, error_descr, sizeof(error_descr));
		std::string e;
		e = "Could not write(): ";
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
void kill_all_connections(EV_P) {
	typeof(c_network.begin()) i;
	while( i = c_network.begin(),  i != c_network.end() ) {
		ev_io_stop(EV_A_ &i->watcher );
		c_network.erase(i);
	}
}


void stop_all_watchers(EV_P) {
	for( typeof(c_network.begin()) i = c_network.begin(); i != c_network.end(); ++i ) {
		ev_io_stop(EV_A_ &i->watcher );
	}
}
void start_all_watchers(EV_P) {
	for( typeof(c_network.begin()) i = c_network.begin(); i != c_network.end(); ++i ) {
		ev_io_start(EV_A_ &i->watcher );
	}
}

void received_sigint(EV_P_ ev_signal *w, int revents) throw() {
	*log << "Received SIGINT, exiting\n" << std::flush;
	ev_unloop(EV_A_ EVUNLOOP_ALL);
}

void ready_to_read(EV_P_ ev_io *w, int revents) throw() {
	struct connection *c = reinterpret_cast<struct connection*>(w->data);

	assert( serial.rx_ready || c == &serial.conn );

	std::string buf;
	try {
		buf = read(c->sock);

	} catch( IOError &e ) {
		*log << c->id << " : IO error, closing connection: " << e.what() << "\n" << std::flush;
		if( c == &serial.conn ) throw;
		kill_connection(EV_A_ w);
		return; // early

	} catch( EOFreached &e ) {
		*log << c->id << " : Disconnect\n" << std::flush;
		if( c == &serial.conn ) throw;
		kill_connection(EV_A_ w);
		return; // early
	}

	c->buf.append(buf);
	while(1) {
		std::auto_ptr<VelbusMessage::VelbusMessage> m;
		try {
			m.reset( VelbusMessage::parse_and_consume(c->buf) );
			*log << c->id << " : " << m->string() << "\n" << std::flush;

		} catch( VelbusMessage::InsufficientData &e ) {
			break; // out of while, and wait for more data
		} catch( VelbusMessage::FormError &e ) {
			*log << c->id << " : Form Error in data (" << e.details() << "), ignoring byte "
			     << "0x" << hex(c->buf[0]) << "\n" << std::flush;
			c->buf = c->buf.substr(1);
			continue; // retry from next byte
		}

		if( c == &serial.conn ) {
			// Do flow control for serial line
			if( typeid(*m) ==typeid(VelbusMessage::BusOff) ) {
				serial.bus_active = false;
				kill_all_connections(EV_A);

			} else if( typeid(*m) ==typeid(VelbusMessage::BusActive) ) {
				serial.bus_active = true;

			} else if( typeid(*m) ==typeid(VelbusMessage::RxBuffFull) ) {
				serial.rx_ready = false;
				stop_all_watchers(EV_A);
				// TODO: Do we need to check for pending callbacks?

			} else if( typeid(*m) ==typeid(VelbusMessage::RxReady) ) {
				serial.rx_ready = true;
				start_all_watchers(EV_A);
			}

		}

		if( c != &serial.conn ) {
			try {
				write(serial.conn.sock, m->message());
			} catch( IOError &e ) {
				throw;
			}
		}
		for( typeof(c_network.begin()) i = c_network.begin(); i != c_network.end(); ++i ) {
			if( &(*i) == c ) continue; // Don't loop input to same socket
			try {
				write(i->sock, m->message());
			} catch( IOError &e ) {
				*log << i->id << " : IO error, closing connection: " << e.what() << "\n" << std::flush;
				ev_io *w = &( i->watcher );
				--i; // Prepare iterator for deletion
				kill_connection(EV_A_ w );
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

	if( ! serial.bus_active ) {
		return; // Without keeping the connection => close it
	}

	// Set socket non-blocking
	int flags = fcntl(new_con->sock, F_GETFL);
	if( flags == -1 ) {
		char error_descr[256];
		strerror_r(errno, error_descr, sizeof(error_descr));
		*log << new_con->id << " : Could not fcntl(, F_GETFL): " << error_descr << "\n" << std::flush;
		*log << new_con->id << " : Closing connection\n" << std::flush;
		return; // Without setting watcher & without keeping connection
	}
	int rv = fcntl(new_con->sock, F_SETFL, flags | O_NONBLOCK);
	if( rv == -1 ) {
		char error_descr[256];
		strerror_r(errno, error_descr, sizeof(error_descr));
		*log << new_con->id << " : Could not fcntl(, F_SETFL): " << error_descr << "\n" << std::flush;
		*log << new_con->id << " : Closing connection\n" << std::flush;
		return; // Without setting watcher & without keeping connection
	}

	ev_io_init( &new_con->watcher, ready_to_read, new_con->sock, EV_READ );
	new_con->watcher.data = new_con.get();
	ev_io_start( EV_A_ &new_con->watcher );

	c_network.push_back( new_con.release() );
}

int main(int argc, char* argv[]) {
	// Defaults
	struct {
		bool fork;
		std::string pid_file;
		std::string serial_port;
		std::string bind_addr;
	} options = {
		/* fork = */ true,
		/* pid_file = */ "",
		/* serial_port = */ "/dev/ttyS0",
		/* bind_addr = */ "[::1]:[8445]"
		};

	log.reset( new TimestampLog( std::cerr ) );

	{ // Parse options
		char optstring[] = "?hfp:s:b:";
		struct option longopts[] = {
			{"help",			no_argument, NULL, '?'},
			{"forgeground",		no_argument, NULL, 'f'},
			{"pid-file",		required_argument, NULL, 'p'},
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
				std::cerr <<
				//  >---------------------- Standard terminal width ---------------------------------<
					"Options:\n"
					"  -h -? --help                    Displays this help message and exits\n"
					"  --foreground -f                 Don't fork into the background after init\n"
					"  --serialport -s  /dev/ttyS0     The serial device to use\n"
					"  --bind -b host:port             Bind to the specified address\n"
					"                                  host and port resolving can be bypassed by\n"
					"                                  placing [] around them\n"
					;
				exit(EX_USAGE);
			case 'f':
				options.fork = false;
				break;
			case 'p':
				options.pid_file = optarg;
				break;
			case 's':
				options.serial_port = optarg;
				break;
			case 'b':
				options.bind_addr = optarg;
				break;
			}
		}
	}

	{ // Open serial port
		serial.conn.id = "SERIAL";
		serial.conn.sock = open(options.serial_port.c_str(), O_RDWR | O_NOCTTY);
		// Open in Read-Write; don't become controlling TTY
		if( serial.conn.sock == -1 ) {
			std::cerr << "Could not open \"" << options.serial_port << "\": ";
			perror("open()");
			exit(EX_NOINPUT);
		}
		*log << "Opened port \"" << options.serial_port << "\"\n" << std::flush;

		// Setting up port
		struct termios port_options;
		tcgetattr(serial.conn.sock, &port_options);

		cfsetispeed(&port_options, B38400);
		cfsetospeed(&port_options, B38400);

		port_options.c_cflag |= CLOCAL; // Don't change owner of port
		port_options.c_cflag |= CREAD; // Enable receiver

		port_options.c_cflag &= ~CSIZE; // Mask the character size bits
		port_options.c_cflag |= CS8;    // Select 8 data bits

		port_options.c_cflag &= ~PARENB; // Clear parity
		port_options.c_cflag &= ~CSTOPB; // Clear "2 Stopbits" => 1 stopbit

		port_options.c_cflag &= ~CRTSCTS; // Disable hardware flow control

		port_options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Raw mode

		port_options.c_iflag |= IGNPAR; // Ignore parity (since none is used)
		port_options.c_iflag &= ~(IXON | IXOFF | IXANY); // Disable software flow control

		port_options.c_oflag &= ~OPOST; // Disable output processing = raw mode

		// Apply port_options
		tcsetattr(serial.conn.sock, TCSANOW, &port_options);

		// Manually setting RTS high & DTR low
		int status;
		ioctl(serial.conn.sock, TIOCMGET, &status); // Get MODEM-bits
		status &= ~TIOCM_DTR; // DTR = 0
		status |= TIOCM_RTS; // RTS = 1
		ioctl(serial.conn.sock, TIOCMSET, &status); // Write MODEM-bits

		*log << "Configured port \"" << options.serial_port << "\"\n" << std::flush;
	}

	{ // Open listening socket
		std::string host, port;

		/* Address format is
		 *   - hostname:portname
		 *   - [numeric ip]:portname
		 *   - hostname:[portnumber]
		 *   - [numeric ip:[portnumber]
		 */
		size_t c = options.bind_addr.rfind(":");
		if( c == std::string::npos ) {
			std::cerr << "Invalid bind string \"" << options.bind_addr << "\": could not find ':'\n";
			exit(EX_DATAERR);
		}
		host = options.bind_addr.substr(0, c);
		port = options.bind_addr.substr(c+1);

		std::auto_ptr< boost::ptr_vector< SockAddr::SockAddr> > bind_sa
			= SockAddr::resolve( host, port, 0, SOCK_STREAM, 0);
		if( bind_sa->size() == 0 ) {
			std::cerr << "Can not bind to \"" << options.bind_addr << "\": Could not resolve\n";
			exit(EX_DATAERR);
		} else if( bind_sa->size() > 1 ) {
			// TODO: allow this
			std::cerr << "Can not bind to \"" << options.bind_addr << "\": Resolves to multiple entries:\n";
			for( typeof(bind_sa->begin()) i = bind_sa->begin(); i != bind_sa->end(); i++ ) {
				std::cerr << "  " << i->string() << "\n";
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
		/* Open pid-file before fork()
		 * That way, failing to open the pid-file will cause a pre-fork-abort
		 */
		std::ofstream pid_file;
		if( options.pid_file.length() > 0 ) pid_file.open( options.pid_file.c_str() );

		if( options.fork ) {
			pid_t child = fork();
			if( child == -1 ) {
				char error_descr[256];
				strerror_r(errno, error_descr, sizeof(error_descr));
				*log << "Could not fork: " << error_descr << "\n" << std::flush;
				exit(EX_OSERR);
			} else if( child == 0 ) {
				// We are the child
				// continue on with the program
			} else {
				// We are the parent
				exit(0);
			}
		}

		if( options.pid_file.length() > 0 ) pid_file << getpid();
	}

	{ // Initialize the serial state
		serial.bus_active = false;
		serial.rx_ready = false;
		VelbusMessage::IntStatusRequest m;
		write(serial.conn.sock, m.message());
		// This will trigger a combination of (BusActive,BusOff)x(RxReady,RxBuffFull)
	}

	{
		ev_signal ev_signal_watcher;
		ev_signal_init( &ev_signal_watcher, received_sigint, SIGINT);
		ev_signal_start( EV_DEFAULT_ &ev_signal_watcher);

		ev_io_init( &serial.conn.watcher, ready_to_read, serial.conn.sock, EV_READ );
		serial.conn.watcher.data = &serial.conn;
		ev_io_start( EV_DEFAULT_ &serial.conn.watcher );

		ev_io e_listen;
		ev_io_init( &e_listen, incomming_connection, s_listen, EV_READ );
		ev_io_start( EV_DEFAULT_ &e_listen );

		*log << "Setup done, starting event loop\n" << std::flush;

		ev_loop(EV_DEFAULT_ 0);
	}

	return 0;
}
