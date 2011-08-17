AC_INIT(velbusd.cpp)
AC_CONFIG_HEADER(config.h)     dnl output results to config.h

AC_PROG_CXX     dnl Check for C++ compiler
AC_LANG_CPLUSPLUS     dnl Test the C++ compiler

dnl Check for the sin_len field in sockaddr_in
AC_CHECK_MEMBER([struct sockaddr_in.sin_len],
	[AC_DEFINE(SOCKADDR_HAS_LEN_FIELD)],
	[],
	[#include <netinet/in.h>])

AC_OUTPUT
