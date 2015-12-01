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

bin_PROGRAMS= \
	ConstructHierarchySelectionTest \
	IdentifierSelectorTest \
	ConstructElevatorTest \
	BooleanCompoundSelectorTest \
	MatcherUnitTest

libInstRO_test_la_SOURCES= \
	$(TEST_SFILES) \
	$(TEST_HFILES)

libInstRO_test_la_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

# Tests independent of the used compiler
MatcherUnitTest_SOURCES= \
	./unittest/MatcherTest.cpp

MatcherUnitTest_LDADD=$(top_builddir)/lib/libInstRO_core.la
MatcherUnitTest_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

# Tests depending on the compiler infrastructure
# This is the actual test executable
ConstructHierarchySelectionTest_SOURCES= \
	./ConstructHierarchySelectionTestMain.cpp

ConstructHierarchySelectionTest_LDADD=libInstRO_test.la $(top_builddir)/lib/libInstRO_core.la
ConstructHierarchySelectionTest_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@


IdentifierSelectorTest_SOURCES= \
	./IdentifierSelectorTest.cpp

IdentifierSelectorTest_LDADD=libInstRO_test.la $(top_builddir)/lib/libInstRO_core.la
IdentifierSelectorTest_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

ConstructElevatorTest_SOURCES= \
	./ConstructElevatorSelectorTest.cpp

ConstructElevatorTest_LDADD=libInstRO_test.la $(top_builddir)/lib/libInstRO_core.la
ConstructElevatorTest_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

BooleanCompoundSelectorTest_SOURCES= \
	./BooleanCompoundSelectorTest.cpp

BooleanCompoundSelectorTest_LDADD=libInstRO_test.la $(top_builddir)/lib/libInstRO_core.la
BooleanCompoundSelectorTest_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

if USEROSE
ConstructHierarchySelectionTest_CPPFLAGS+= -DUSE_ROSE -I$(top_srcdir)/../lib/include -I@ROSE_INCLUDEDIR@
ConstructHierarchySelectionTest_LDADD+= $(top_builddir)/lib/libInstRO_rose.la @ROSE_LINK_LIBRARIES@ -L@BOOST_LIBDIR@

IdentifierSelectorTest_CPPFLAGS+= -DUSE_ROSE -I$(top_srcdir)/../lib/include -I@ROSE_INCLUDEDIR@
IdentifierSelectorTest_LDADD+= $(top_builddir)/lib/libInstRO_rose.la @ROSE_LINK_LIBRARIES@ -L@BOOST_LIBDIR@

ConstructElevatorTest_CPPFLAGS+= -DUSE_ROSE -I$(top_srcdir)/../lib/include -I@ROSE_INCLUDEDIR@
ConstructElevatorTest_LDADD+=  $(top_builddir)/lib/libInstRO_rose.la @ROSE_LINK_LIBRARIES@ -L@BOOST_LIBDIR@

BooleanCompoundSelectorTest_CPPFLAGS+= -DUSE_ROSE -I$(top_srcdir)/../lib/include -I@ROSE_INCLUDEDIR@
BooleanCompoundSelectorTest_LDADD+= $(top_builddir)/lib/libInstRO_rose.la @ROSE_LINK_LIBRARIES@ -L@BOOST_LIBDIR@
endif

if USECLANG
# FIXME Untested so far.
ConstructHierarchySelectionTest_LDADD+= $(top_builddir)/lib/libInstRO_clang.la
endif

else #USELIBTOOL

# Static test-library.
# The library provides additional functionality in the instrumentor and the pass factory
lib_LIBRARIES= libInstRO_test.a
libInstRO_test_a_SOURCES= \
	$(TEST_SFILES) \
	$(TEST_HFILES) 
libInstRO_test_a_CPPFLAGS=$(top_srcdir)/lib $(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

# The different test-executables.
# The choice which tests to implement was guided by the "InstRO-mandatory-component-sheet"
bin_PROGRAMS= \
	ConstructHierarchySelectionTest \
	IdentifierSelectorTest \
	ConstructElevatorTest \
	BooleanCompoundSelectorTest \
	MatcherUnitTest

####
# Tests independent of the used compiler
####
# These can be seen as unit-tests as those test very specific small things like single classes.
MatcherUnitTest_SOURCES= \
	./unittest/MatcherTest.cpp
MatcherUnitTest_LDADD=$(top_builddir)/lib/libInstRO_core.a
MatcherUnitTest_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

####
# Test instrumentors
####
#
# In general those executables rely on a compiler implementation as they read input files.
#
# Default flags we use for all our test-executables.
DEFAULT_INSTRO_TEST_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@
DEFAULT_INSTRO_TEST_LDADD=libInstRO_test.a $(top_builddir)/lib/libInstRO_core.a -L@BOOST_LIBDIR@


# Test
ConstructHierarchySelectionTest_SOURCES= \
	./ConstructHierarchySelectionTestMain.cpp
ConstructHierarchySelectionTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD)
ConstructHierarchySelectionTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS)

# Test
IdentifierSelectorTest_SOURCES= \
	./IdentifierSelectorTest.cpp
IdentifierSelectorTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD)
IdentifierSelectorTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS)

# Test
ConstructElevatorTest_SOURCES= \
	./ConstructElevatorSelectorTest.cpp															 
ConstructElevatorTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD)
ConstructElevatorTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS)

# Test
BooleanCompoundSelectorTest_SOURCES= \
	./BooleanCompoundSelectorTest.cpp																		
BooleanCompoundSelectorTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD)
BooleanCompoundSelectorTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS)

# Dependent on the compiler platform we build differently
if USEROSE
INSTRO_WITH_ROSE_CPPFLAGS=-DUSE_ROSE -I$(top_srcdir)/lib -I@ROSE_INCLUDEDIR@
INSTRO_WITH_ROSE_LDADD=$(top_builddir)/lib/libInstRO_rose.a @ROSE_LINK_LIBRARIES@ -L@BOOST_LIBDIR@

ConstructHierarchySelectionTest_CPPFLAGS+= $(INSTRO_WITH_ROSE_CPPFLAGS)
ConstructHierarchySelectionTest_LDADD+= $(INSTRO_WITH_ROSE_LDADD)

IdentifierSelectorTest_CPPFLAGS+= $(INSTRO_WITH_ROSE_CPPFLAGS)
IdentifierSelectorTest_LDADD+= $(INSTRO_WITH_ROSE_LDADD)

ConstructElevatorTest_CPPFLAGS+= $(INSTRO_WITH_ROSE_CPPFLAGS)
ConstructElevatorTest_LDADD+= $(INSTRO_WITH_ROSE_LDADD)

BooleanCompoundSelectorTest_CPPFLAGS+= $(INSTRO_WITH_ROSE_CPPFLAGS)
BooleanCompoundSelectorTest_LDADD+=  $(INSTRO_WITH_ROSE_LDADD)
endif

# In order to run 'make check'
check:
	./MatcherUnitTest
	python $(top_srcdir)/test/ApplyRunner.py $(top_srcdir) $(top_builddir)

endif #USELIBTOOL


