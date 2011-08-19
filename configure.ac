AC_INIT([velbusd],
	[0.0.01],
	[Niobos niobos@dest-unreach.be],
	[velbusd])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADERS(config.h)     dnl output results to config.h
AM_INIT_AUTOMAKE([dist-bzip2 subdir-objects -Wall])

# Options
#########


# Checks for programs
#####################
AC_PROG_CC
AC_PROG_CXXCPP        dnl Check for C++ preprocessor
AC_PROG_CXX           dnl Check for C++ compiler
AC_LANG_CPLUSPLUS     dnl Test the C++ compiler
AC_C_INLINE

# Library checks
################


# Header checks
###############
AC_CHECK_HEADERS([arpa/inet.h])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([netdb.h])
AC_CHECK_HEADERS([netinet/in.h])
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([string.h])
AC_CHECK_HEADERS([strings.h])
AC_CHECK_HEADERS([sys/ioctl.h])
AC_CHECK_HEADERS([sys/socket.h])
AC_CHECK_HEADERS([sys/time.h])
AC_CHECK_HEADERS([termios.h])
AC_CHECK_HEADERS([unistd.h])
AC_HEADER_STDBOOL


# Typedefs & structs
####################
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Check for the sin_len field in sockaddr_in
AC_CHECK_MEMBER([struct sockaddr_in.sin_len],
	[AC_DEFINE([SOCKADDR_HAS_LEN_FIELD], [], [struct sockaddr_in has sin_len field])],
	[],
	[#include <netinet/in.h>])

# Functions
###########
AC_CHECK_FUNCS([bzero])
AC_CHECK_FUNCS([socket])
AC_CHECK_FUNCS([strerror])
AC_FUNC_STRERROR_R

# Output
########
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	test/Makefile
	])

AC_OUTPUT
