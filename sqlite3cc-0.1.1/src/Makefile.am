# Makefile.am -- process this file with automake to produce Makefile.in

AM_CPPFLAGS = -I$(top_srcdir)/include

lib_LTLIBRARIES = libsqlite3cc.la

libsqlite3cc_la_SOURCES = \
	connection.cc \
	exception.cc \
	basic_statement.cc \
	command.cc \
	query.cc \
	row.cc \
	transaction.cc \
	manipulator.cc \
	util.cc
libsqlite3cc_la_LIBADD = \
	-lboost_thread \
	-lsqlite3
libsqlite3cc_la_LDFLAGS = -version-info @LIBSQLITE3CC_INTERFACE@
