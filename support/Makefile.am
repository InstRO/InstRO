AM_CPPFLAGS=-I$(top_srcdir)/lib/include -I$(top_builddir)/lib/include -DPROVIDE_MAKE_UNIQUE

AM_CXXFLAGS=-std=c++11
AM_CFLAGS=

if DEBUG
AM_CFLAGS += -g3 -O0 -Wall
AM_CXXFLAGS += -g3 -O0 -Wall
else
AM_CFLAGS += -O2
AM_CXXFLAGS += -O2
endif

# We want to build a shared library for InstRO Measurement Interface
lib_LTLIBRARIES = libimi.la

libimi_la_SOURCES = InstROMeasurementInterface.c
include_HEADERS = InstROMeasurementInterface.h

