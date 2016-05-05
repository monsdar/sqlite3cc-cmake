# Makefile.am -- process this file with automake to produce Makefile.in

CPPFLAGS = -I$(top_srcdir)/include

check_PROGRAMS = test-main

test_main_SOURCES = \
	test-main.cc
test_main_LDADD = \
	../src/libsqlite3cc.la \
	-lboost_filesystem \
	-lboost_system

TESTS = test-main
