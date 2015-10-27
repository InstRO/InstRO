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
bin_PROGRAMS=ExampleInstrumentor

if USEROSE
bin_PROGRAMS += RoseInstrumentor ROSEInstROASTPrinter
endif

if HAVERAPIDJSON
SUBDIRS= ConfigurationLoader
endif


RoseInstrumentor_SOURCES = \
	./roseInstrumentor.cpp
RoseInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@
RoseInstrumentor_LDADD=$(top_builddir)/lib/libInstRO_rose.la @ROSE_LINK_LIBRARIES@

ExampleInstrumentor_SOURCES = \
	./example_main.cpp

# TODO Is this actually an example?
ROSEInstROASTPrinter_SOURCES = \
	./main.cxx
ROSEInstROASTPrinter_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@
ROSEInstROASTPrinter_LDADD=$(top_builddir)/lib/libInstRO_rose.la @ROSE_LINK_LIBRARIES@ -L@BOOST_LIBDIR@

ExampleInstrumentor_LDADD=$(top_builddir)/lib/libInstRO_example.la
ExampleInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@  -L@BOOST_LIBDIR@

else # USELIBTOOL

bin_PROGRAMS=ExampleInstrumentor 

if USEROSE
bin_PROGRAMS += RoseInstrumentor ROSEInstROASTPrinter
endif

if HAVERAPIDJSON
SUBDIRS= ConfigurationLoader
endif


RoseInstrumentor_SOURCES = \
	./roseInstrumentor.cpp

RoseInstrumentor_LDADD=$(top_builddir)/lib/libInstRO_core.a $(top_builddir)/lib/libInstRO_rose.a
RoseInstrumentor_LDFLAGS=-L@ROSE_LIBDIR@ @ROSE_LINK_LIBRARIES@  -L@BOOST_LIBDIR@
RoseInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@

# TODO Is this actually an example?
ROSEInstROASTPrinter_SOURCES = \
	./main.cxx
ROSEInstROASTPrinter_LDADD=$(top_builddir)/lib/libInstRO_core.a $(top_builddir)/lib/libInstRO_rose.a
ROSEInstROASTPrinter_LDFLAGS=-L@ROSE_LIBDIR@ @ROSE_LINK_LIBRARIES@ -L@BOOST_LIBDIR@
ROSEInstROASTPrinter_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@

ExampleInstrumentor_SOURCES = \
	./example_main.cpp

ExampleInstrumentor_LDADD= $(top_builddir)/lib/libInstRO_example.a
ExampleInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

## Manually crafted Clang stuff!
# FIXME Figure out how this can be done using actually autotools
ClangInstRO:
	$(CXX) $(LLVM_CXXFLAGS_FLAGS) -I$(LLVM_SRC_INCLUDE_PATH) -I$(CLANG_SRC_INCLUDE_PATH) -I$(BOOST_INCLUDEDIR) $(AM_CPPFLAGS) $(top_srcdir)/lib/src/clang/Instrumentor.cpp $(top_builddir)/lib/libInstRO_clang.a $(top_builddir)/lib/libInstRO_core.a $(CLANG_LIBS_LIST) $(LLVM_LDFLAGS) -o $@


endif # USELIBTOOL
