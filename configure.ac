AC_INIT([velbusd],
	[0.0.01],
	[Niobos niobos@dest-unreach.be],
	[velbusd])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_HEADERS(config.h)     dnl output results to config.h
AM_INIT_AUTOMAKE([dist-bzip2 subdir-objects -Wall])

# Options
#########
AC_ARG_ENABLE([parser],
	[  --enable-parser	Include parser modules (default enabled)],
	[case "${enableval}" in
	  yes) enable_parser=true;;
	  no)  enable_parser=false;;
	  *)   AC_MSG_ERROR([bad value ${enableval} for --enable-parser]) ;;
	esac],[enable_parser=true])
AM_CONDITIONAL([WANT_PARSER], [test x$enable_parser = xtrue])


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
	src/Makefile
	test/Makefile
	])

AC_OUTPUT
