#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>

static const size_t READ_SIZE = 4096;

int read_write(int from, int to) {
	int rv;
	char buf[READ_SIZE];
	int n = read(from, buf, READ_SIZE);
	if( n == -1 ) {
		perror("read()");
		return 1;
	} else if( n == 0 ) {
		return 2;
	}
	fprintf(stderr, "read %d bytes\n", n);
	rv = write(to, buf, n);
	if( rv == -1 ) {
		perror("write()");
		return 1;
	} else if( rv != n ) {
		fprintf(stderr, "incorrect number of bytes written\n");
		return 1;
	}
	return 0;
}

int main(int argc, char* argv[]) {
	int fd_ser;
	struct termios options;
	int status;
	
	fprintf(stderr, "Opening serial port\n");
	fd_ser = open("/dev/ttyS0", O_RDWR | O_NOCTTY);
	// Open serial port in Read-write, don't be the controlling TTY and ignore DCD line
	if( fd_ser == -1 ) {
		perror("Couldn't open serial port");
		return 1;
	}

	fprintf(stderr, "Port opened, setting 38400 8N1\n");
	tcgetattr(fd_ser, &options);

	// Change options
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

	fprintf(stderr, "UART options set; manually setting RTS & DTR\n");
	// Set RTS & DTR
	ioctl(fd_ser, TIOCMGET, &status); // Get MODEM-bits
	status &= ~TIOCM_DTR;
	status |= TIOCM_RTS;
	ioctl(fd_ser, TIOCMSET, &status); // Write MODEM-bits

	{ // Select loop
		int rv;
		int fd_max = fd_ser;
		fd_set rfds;
		FD_ZERO(&rfds);
		FD_SET(0, &rfds); // STDIN
		FD_SET(fd_ser, &rfds);

		fprintf(stderr, "Entering select\n");
		while( rv = select( fd_max+1, &rfds, NULL, NULL, NULL) ) {
			if( rv == -1 ) {
				perror("select()");
				return 1;
			} else if( rv == 0 ) {
				fprintf(stderr, "Weird: select() returned 0. retrying");
			} else {
				if( FD_ISSET(0, &rfds) ) {
					// STDIN is ready to read
					int rv;
					fprintf(stderr, "ser->stdout\n");
					rv = read_write(0, fd_ser);
					if( rv == 1 ) { return 1; }
					else if( rv == 2 ) break;
				}
				if( FD_ISSET(fd_ser, &rfds) ) {
					int rv;
					fprintf(stderr, "ser->stdout\n");
					rv = read_write(fd_ser, 1);
					if( rv == 1 ) { return 1; }
					else if( rv == 2 ) break;
				}
			}

		FD_ZERO(&rfds);
		FD_SET(0, &rfds); // STDIN
		FD_SET(fd_ser, &rfds);
		}
	}

	close(fd_ser);
	return 0;
}
