AM_CPPFLAGS=-I$(top_srcdir)/lib/include -I$(top_builddir)/lib/include -DPROVIDE_MAKE_UNIQUE

AM_CXXFLAGS=-std=c++11
AM_CFLAGS=

if DEBUG
AM_CFLAGS += -g3 -O0 -Wall
AM_CXXFLAGS += -g3 -O0 -Wall
else
AM_CFLAGS += -g -O2
AM_CXXFLAGS += -g -O2
endif


TEST_SFILES= \
						 ./lib/TestAdapter.cpp

TEST_HFILES= \
						 ./lib/TestAdapter.h

if USELIBTOOL
# LIBTOOL RULE FOR LIBINSTRO
lib_LTLIBRARIES= libInstRO_test.la

bin_PROGRAMS= ConstructHierarchySelectionTest

libInstRO_test_la_SOURCES= \
	$(TEST_SFILES) \
	$(TEST_HFILES)

libInstRO_test_la_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

# This is the actual test executable
ConstructHierarchySelectionTest_SOURCES= \
	./ConstructHierarchySelectionTestMain.cpp

ConstructHierarchySelectionTest_LDADD=libInstRO_test.la $(top_builddir)/lib/libInstRO_core.la
ConstructHierarchySelectionTest_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

if USEROSE
ConstructHierarchySelectionTest_CPPFLAGS+= -I$(top_srcdir)/../lib/include -I@ROSE_INCLUDEDIR@
ConstructHierarchySelectionTest_LDADD+= $(top_builddir)/lib/libInstRO_rose.la @ROSE_LINK_LIBRARIES@
endif

if USECLANG
# FIXME Untested so far.
ConstructHierarchySelectionTest_LDADD+= $(top_builddir)/lib/libInstRO_clang.la
endif

else #USELIBTOOL

# STATIC LIBRARY FOR LIBINSTRO
lib_LIBRARIES= libInstRO_test.a

bin_PROGRAMS= ConstructHierarchySelectionTest

libInstRO_test_a_SOURCES= \
	$(TEST_SFILES) \
	$(TEST_HFILES) 

libInstRO_test_a_CPPFLAGS=$(top_srcdir)/lib $(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

# This is the actual test executable
ConstructHierarchySelectionTest_SOURCES= \
	./ConstructHierarchySelectionTestMain.cpp

ConstructHierarchySelectionTest_LDADD=libInstRO_test.a $(top_builddir)/lib/libInstRO_core.a -L@BOOST_LIBDIR@
ConstructHierarchySelectionTest_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

if USEROSE
ConstructHierarchySelectionTest_CPPFLAGS+= -DUSE_ROSE -I$(top_srcdir)/lib -I@ROSE_INCLUDEDIR@
ConstructHierarchySelectionTest_LDADD+= $(top_builddir)/lib/libInstRO_rose.a @ROSE_LINK_LIBRARIES@ -L@BOOST_LIBDIR@
endif

check:
	python $(top_srcdir)/test/ApplyRunner.py $(top_srcdir) $(top_builddir)

endif #USELIBTOOL


