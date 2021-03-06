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

RT_SUPPORT_HFILES= \
	../support/InstROMeasurementInterface.h
RT_SUPPORT_SFILES= \
	./IMITestInfrastructure.c

lib_LTLIBRARIES= libInstRO_test.la libInstRO_rtsupport.la libInstRO_rtsupport_dbg.la


libInstRO_test_la_SOURCES= \
	$(TEST_SFILES) \
	$(TEST_HFILES)
libInstRO_test_la_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

libInstRO_rtsupport_la_SOURCES= \
	$(RT_SUPPORT_SFILES) \
	$(RT_SUPPORT_HFILES)
libInstRO_rtsupport_la_CFLAGS=-I$(top_srcdir)/support $(AM_CPPFLAGS)

libInstRO_rtsupport_dbg_la_SOURCES= \
	$(RT_SUPPORT_SFILES) \
	$(RT_SUPPORT_HFILES)
libInstRO_rtsupport_dbg_la_CFLAGS=-I$(top_srcdir)/support $(AM_CPPFLAGS) -DCDEBUG

# Default flags we use for all our test-executables.
DEFAULT_INSTRO_TEST_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

DEFAULT_INSTRO_TEST_LDADD=libInstRO_test.la $(top_builddir)/lib/libInstRO_core.la @BOOST_LINK_LIBRARIES@

# The variable is used to pass a list of optional parameters/information to the runner script
OPTIONALS_LIST=''

bin_PROGRAMS= \
	MatcherUnitTest \
	ConstructTraitTypeTest

if USEROSE
bin_PROGRAMS+= \
	ConstructHierarchySelectionTest \
	IdentifierSelectorTest \
	ConstructElevatorTest \
	BooleanCompoundSelectorTest \
	CallpathSelectorTest \
	DefaultInstrumentationAdapterTest \
	UniqueCallpathTransformerTest \
	FunctionWrapperTest

# There should be some nicer way instead of those nested conditional things...
if OPT_SCOREP
bin_PROGRAMS+= \
	ScorepRegionAdapterTest

OPTIONALS_LIST+='scorep'
endif

endif

if USECLANG
bin_PROGRAMS+= \
	ConstructHierarchySelectionTest
endif

# Tests independent of the used compiler
MatcherUnitTest_SOURCES= \
	./unittest/MatcherTest.cpp

MatcherUnitTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD)
MatcherUnitTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS) -I./unittest

ConstructTraitTypeTest_SOURCES= \
	./unittest/ConstructTraitTypeTest.cpp

ConstructTraitTypeTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD)
ConstructTraitTypeTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS) -I./unittest


# Tests depending on the compiler infrastructure
# This is the actual test executable
ConstructHierarchySelectionTest_SOURCES= \
	./ConstructHierarchySelectionTestMain.cpp

ConstructHierarchySelectionTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD)
ConstructHierarchySelectionTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS)


IdentifierSelectorTest_SOURCES= \
	./IdentifierSelectorTest.cpp

IdentifierSelectorTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD)
IdentifierSelectorTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS)

ConstructElevatorTest_SOURCES= \
	./ConstructElevatorSelectorTest.cpp

ConstructElevatorTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD)
ConstructElevatorTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS)

BooleanCompoundSelectorTest_SOURCES= \
	./BooleanCompoundSelectorTest.cpp

BooleanCompoundSelectorTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD)
BooleanCompoundSelectorTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS)

DefaultInstrumentationAdapterTest_SOURCES= \
	./DefaultInstrumentationAdapterTest.cpp

DefaultInstrumentationAdapterTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD)
DefaultInstrumentationAdapterTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS)


# Test
CallpathSelectorTest_SOURCES= \
	./CallpathSelectorTest.cpp
CallpathSelectorTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD)
CallpathSelectorTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS)

if USEROSE

INSTRO_WITH_ROSE_CPPFLAGS= -DUSE_ROSE -I$(top_srcdir)/../lib/include -I@ROSE_INCLUDEDIR@
INSTRO_WITH_ROSE_LDADD= $(top_builddir)/lib/libInstRO_rose.la @ROSE_LINK_LIBRARIES@ @BOOST_LINK_LIBRARIES@

ConstructHierarchySelectionTest_CPPFLAGS+= $(INSTRO_WITH_ROSE_CPPFLAGS)
ConstructHierarchySelectionTest_LDADD+= $(INSTRO_WITH_ROSE_LDADD)

IdentifierSelectorTest_CPPFLAGS+= $(INSTRO_WITH_ROSE_CPPFLAGS)
IdentifierSelectorTest_LDADD+= $(INSTRO_WITH_ROSE_LDADD)

ConstructElevatorTest_CPPFLAGS+= $(INSTRO_WITH_ROSE_CPPFLAGS)
ConstructElevatorTest_LDADD+= $(INSTRO_WITH_ROSE_LDADD)

BooleanCompoundSelectorTest_CPPFLAGS+= $(INSTRO_WITH_ROSE_CPPFLAGS)
BooleanCompoundSelectorTest_LDADD+= $(INSTRO_WITH_ROSE_LDADD)

DefaultInstrumentationAdapterTest_CPPFLAGS+= $(INSTRO_WITH_ROSE_CPPFLAGS)
DefaultInstrumentationAdapterTest_LDADD+= $(INSTRO_WITH_ROSE_LDADD)

CallpathSelectorTest_CPPFLAGS+= $(INSTRO_WITH_ROSE_CPPFLAGS)
CallpathSelectorTest_LDADD+= $(INSTRO_WITH_ROSE_LDADD)

UniqueCallpathTransformerTest_SOURCES=./UniqueCallpathTransformerTestMain.cpp
UniqueCallpathTransformerTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD) $(INSTRO_WITH_ROSE_LDADD)
UniqueCallpathTransformerTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS) $(INSTRO_WITH_ROSE_CPPFLAGS)

FunctionWrapperTest_SOURCES=./FunctionWrapperTestMain.cpp
FunctionWrapperTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD) $(INSTRO_WITH_ROSE_LDADD)
FunctionWrapperTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS) $(INSTRO_WITH_ROSE_CPPFLAGS)

if OPT_SCOREP
ScorepRegionAdapterTest_SOURCES= \
	./ScorepRegionAdapterTest.cpp
ScorepRegionAdapterTest_LDADD=$(DEFAULT_INSTRO_TEST_LDADD)
ScorepRegionAdapterTest_CPPFLAGS=$(DEFAULT_INSTRO_TEST_CPPFLAGS)

ScorepRegionAdapterTest_CPPFLAGS+= $(INSTRO_WITH_ROSE_CPPFLAGS)
ScorepRegionAdapterTest_LDADD+= $(INSTRO_WITH_ROSE_LDADD)
endif # OPT_SCOREP


INSTRO_COMPILER_INDICATION="rose"

endif

if USECLANG

INSTRO_COMPILER_INDICATION="clang"

ConstructHierarchySelectionTest: libInstRO_test.la $(top_srcdir)/test/lib/ClangTestSupport.h $(top_srcdir)/test/ConstructHierarchySelectionTestMain.cpp $(top_builddir)/lib/.libs/libInstRO_clang.la $(top_builddir)/lib/.libs/libInstRO_core.la
		$(CXX) $(LLVM_CXXFLAGS_FLAGS) -I$(LLVM_SRC_INCLUDE_PATH) -I$(CLANG_SRC_INCLUDE_PATH) -I$(BOOST_INCLUDEDIR) $(AM_CPPFLAGS) -DCDEBUG -I$(top_srcdir)/test $(top_srcdir)/test/ConstructHierarchySelectionTestMain.cpp -L.libs -lInstRO_test -L$(top_builddir)/lib/.libs -lInstRO_clang -lInstRO_core @BOOST_LINK_LIBRARIES@ $(CLANG_LIBS_LIST) $(LLVM_LDFLAGS) -o $@

endif



# In order to run 'make check'
#
unittests: MatcherUnitTest ConstructTraitTypeTest
	./MatcherUnitTest
	./ConstructTraitTypeTest

check: unittests
	python $(top_srcdir)/test/ApplyRunner.py $(top_srcdir) $(top_builddir) $(INSTRO_COMPILER_INDICATION) $(OPTIONALS_LIST)


