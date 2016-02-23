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


if HAVERAPIDJSON
bin_PROGRAMS = ConfigInstrumentor
endif

ConfigInstrumentor_SOURCES=./main.cpp
ConfigInstrumentor_CPPFLAGS= -DUSE_ROSE $(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@ -I@ROSE_INCLUDEDIR@ -I@RAPIDJSON_INCLUDEDIR@
ConfigInstrumentor_LDADD=$(top_builddir)/lib/libInstRO_rose.la @ROSE_LINK_LIBRARIES@  -L@BOOST_LIBDIR@

