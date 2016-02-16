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

if USELIBTOOL
bin_PROGRAMS=

if USEROSE
bin_PROGRAMS += RoseInstrumentor ROSEInstROASTPrinter RoseCallpathToKernelInstrumentor

if HAVERAPIDJSON
SUBDIRS= ConfigurationLoader
endif

endif #USEROSE

if USECLANG
SUBDIRS= ClangSrcToSrcFunctionInstrumentor
endif

RoseInstrumentor_SOURCES = \
	./RoseInstrumentor.cpp
RoseInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@
RoseInstrumentor_LDADD=$(top_builddir)/lib/libInstRO_rose.la @ROSE_LINK_LIBRARIES@

RoseCallpathToKernelInstrumentor_SOURCES= \
	./RoseCallpathToKernelInstrumentor.cpp
RoseCallpathToKernelInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@
RoseCallpathToKernelInstrumentor_LDADD=$(top_builddir)/lib/libInstRO_rose.la @ROSE_LINK_LIBRARIES@ -L@BOOST_LIBDIR@

# TODO Is this actually an example?
ROSEInstROASTPrinter_SOURCES = \
	./RoseConstructASTExporter.cpp
ROSEInstROASTPrinter_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@
ROSEInstROASTPrinter_LDADD=$(top_builddir)/lib/libInstRO_rose.la @ROSE_LINK_LIBRARIES@ -L@BOOST_LIBDIR@

else # USELIBTOOL

bin_PROGRAMS=


if USEROSE
bin_PROGRAMS += RoseInstrumentor ROSEInstROASTPrinter RoseCallpathToKernelInstrumentor

if HAVERAPIDJSON
SUBDIRS= ConfigurationLoader
endif

endif #USEROSE

if USECLANG
SUBDIRS= ClangSrcToSrcFunctionInstrumentor
endif

RoseInstrumentor_SOURCES = \
	./RoseInstrumentor.cpp

RoseInstrumentor_LDADD=$(top_builddir)/lib/libInstRO_core.a $(top_builddir)/lib/libInstRO_rose.a
RoseInstrumentor_LDFLAGS=-L@ROSE_LIBDIR@ @ROSE_LINK_LIBRARIES@  -L@BOOST_LIBDIR@
RoseInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@

RoseCallpathToKernelInstrumentor_SOURCES= \
	./RoseCallpathToKernelInstrumentor.cpp
RoseCallpathToKernelInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@
RoseCallpathToKernelInstrumentor_LDADD=$(top_builddir)/lib/libInstRO_rose.a @ROSE_LINK_LIBRARIES@ -L@BOOST_LIBDIR@


# TODO Is this actually an example?
ROSEInstROASTPrinter_SOURCES = \
	./RoseConstructASTExporter.cpp
ROSEInstROASTPrinter_LDADD=$(top_builddir)/lib/libInstRO_core.a $(top_builddir)/lib/libInstRO_rose.a
ROSEInstROASTPrinter_LDFLAGS=-L@ROSE_LIBDIR@ @ROSE_LINK_LIBRARIES@ -L@BOOST_LIBDIR@
ROSEInstROASTPrinter_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@

endif # USELIBTOOL
