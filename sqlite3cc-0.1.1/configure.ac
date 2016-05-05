# configure.ac -- process this file with autoconf to produce a configure script

AC_INIT([sqlite3cc], [0.1.1], [tim@ed.am], , [http://ed.am/dev/sqlite3cc])
AC_SUBST([LIBSQLITE3CC_INTERFACE], [0:0:0])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign dist-xz])
AC_CONFIG_SRCDIR([src/command.cc])
AM_CONFIG_HEADER([config.h])

# checks for programs.
AC_PROG_CXX
AC_PROG_LIBTOOL

# checks for libraries.
AC_CHECK_LIB([sqlite3], sqlite3_initialize, ,
    AC_MSG_ERROR([Unable to find SQLite3 library]))

# checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([string.h])
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([boost/lexical_cast.hpp], [],
	[AC_MSG_ERROR(Boost libraries not found)])
AC_CHECK_HEADERS([boost/thread.hpp], [],
	[AC_MSG_ERROR(Boost threads library not found)])
AC_LANG_POP([C++])

# checks for typedefs, structures, and compiler characteristics.

# checks for library functions.

AC_OUTPUT(Makefile src/Makefile include/Makefile test/Makefile)
