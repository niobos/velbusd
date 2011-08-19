#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

/* Very simple 'cat' replacement that will fail as soon as
 * the write would block
 */

int main() {
	int flags = fcntl(1, F_GETFL);
	if( flags == -1 ) {
		perror("fcntl(F_GETFL)");
		return 1;
	}
	flags |= O_NONBLOCK;
	int rv = fcntl(1, F_SETFL, flags);
	if( rv == -1 ) {
		perror("fcntl(F_SETFL)");
		return 1;
	}

	while(1) {
		fd_set rfds;
		int fd_max;
		FD_ZERO(&rfds);
		FD_SET(0, &rfds); fd_max = 0;
		int rv = select(fd_max+1, &rfds, NULL, NULL, NULL);
		if( rv == -1 ) {
			perror("select()");
			return 1;
		}

		char buf[4096];
		size_t n = read(0, buf, sizeof(buf));
		if( n == -1 ) {
			perror("read()");
			return 1;
		}
		if( n == 0 ) {
			// EOF
			return 0;
		}
		rv = write(1, buf, n);
		if( rv == -1 ) {
			perror("write()");
			return 1;
		}
	}
}
