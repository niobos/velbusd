AC_INIT([velbusd],
	[0.0.01],
	[Niobos niobos@dest-unreach.be],
	[velbusd])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADERS(config.h)     dnl output results to config.h
AM_INIT_AUTOMAKE([dist-bzip2 subdir-objects -Wall])

# Options
#########
AC_ARG_ENABLE([parser],AC_HELP_STRING([--disable-parser],[Disable parsing modules]))
AS_IF([test x$enable_parser != xno], [ AC_DEFINE([WANT_PARSER], [1], [Include parsing modules]) ])
AM_CONDITIONAL([WANT_PARSER], [test x$enable_parser != xno])

AC_DEFINE([WANT_OPTIMISTIC_BUS],[true],[Assume the bus is free on start])
AC_ARG_ENABLE([optimistic-bus], AC_HELP_STRING([--enable-optimistic-bus],[Assume the bus is ready unless we receive the contrary]))
AS_IF([test x$enable_optimistic_bus != xyes], [AC_DEFINE([WANT_OPTIMISTIC_BUS],[false])] )

# Checks for programs
#####################
AC_PROG_CC
AC_PROG_CXXCPP        dnl Check for C++ preprocessor
AC_PROG_CXX           dnl Check for C++ compiler
AC_LANG_CPLUSPLUS     dnl Test the C++ compiler
AC_C_INLINE

# Library checks
################
# Don't check for ev_{loop,run} or ev_{unloop,break} since they changed from libev-3 to libev4
AC_CHECK_LIB(ev, ev_io_start, [], [AC_MSG_ERROR([Couldn't find libev])], [])


# Header checks
###############
AC_CHECK_HEADER([boost/ptr_container/ptr_list.hpp], [], [AC_MSG_ERROR([Couldn't find boost library])], [])
AC_CHECK_HEADERS([pty.h util.h], [break;], [], [])


# Typedefs & structs
####################

# Check for the sin_len field in sockaddr_in
AC_CHECK_MEMBER([struct sockaddr_in.sin_len],
	[AC_DEFINE([SOCKADDR_HAS_LEN_FIELD], [], [struct sockaddr_in has sin_len field])],
	[],
	[#include <netinet/in.h>])

# Functions
###########


# Output
########
AC_CONFIG_FILES([
	Makefile
	bin/Makefile
	src/Makefile
	test/Makefile
	www/Makefile
	])

AC_OUTPUT
