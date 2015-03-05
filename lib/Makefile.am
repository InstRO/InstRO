AM_CPPFLAGS=-I$(top_builddir)/lib/include
AM_CFALGS=-O0

if DEBUG
AM_CFLAGS = -g3 -O0
AM_CXXFLAGS = -g3 -O0 -std=gnu++0x
else
AM_CFLAGS = -O2
AM_CXXFLAGS = -O2 -std=gnu++0x
endif

SFILES=\
./src/core/ConstructSet.cpp \
./src/core/ConstructSetManagement.cpp

HFILES=\
./include/instro.h \
./include/instro/core/ConstructSet.h \
./include/instro/core/ConstructSetManagement.h \
./include/instro/core/Factory.h \
./include/instro/core/Instrumentor.h \
./include/instro/core/Pass.h \
./include/instro/core/PassFactory.h \
./include/instro/core/PassManager.h \
./include/instro/core/ConstructLevelManagrment.h \
./include/instro/core/ConstructElevator.h \
./include/instro/core/PassImplementation.h \
./include/instro/agnosticSupport.h \
./include/instro/misc/VisualizingManager.h \
./include/instro/misc/VisualizingPassManager.h \
./include/instro/rose/RoseInstrumentor.h \
./include/instro/rose/RosePass.h \
./include/instro/rose/RosePassFactory.h \
./include/instro/rose/contrib/geimeradapter.h \
./include/instro/rose/contrib/geimerparser.h \
./include/instro/rose/contrib/geimerselector.h \
./include/instro/rose/selectors/BlackAndWhiteListSelector.h \
./include/instro/rose/selectors/CompoundSelectors.h \
./include/instro/rose/adapters/CygProfileAdapter.h \
./include/instro/rose/adapters/GenericAdapter.h \
./include/instro/roseInstRO.h \
./include/instro/llvmInstRO.h

if USELIBTOOL
# LIBTOOL RULE FOR LIBINSTRO
lib_LTLIBRARIES= libInstRO.la

libInstRO_la_SOURCES=\
$(SFILES) \
$(HFILES)

else
# STATIC LIBRARY FOR LIBINSTRO
lib_LIBRARIES=libInstRO.a

libInstRO_a_SOURCES=\
$(SFILES) \
$(HFILES)

libInstRO_a_CPPFLAGS=-I$(srcdir)/include -I@ROSE_INCLUDEDIR@ -I@ROSE_NEW_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@

endif

include_HEADERS=./include/instro.h

pkginclude_HEADERS=\
$(HFILES)


## libInstRO_la_DEPENDENCIES = @ROSE@
# Add Rose as an dependency
## libInstRO_la_LIBADD= @ROSE@
libInstRO_la_CPPFLAGS=-I$(srcdir)/include -I@ROSE_INCLUDEDIR@ -I@ROSE_NEW_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@
## libInstRO_la_CFLAGS=-I@ROSE_INCLUDEDIR@
## libInstRO_la_CXXFLAGS=-I@ROSE_INCLUDEDIR@
libInstRO_la_LDFLAGS=-L@ROSE_LIBDIR@
# libInstRO_la_CFLAGS=-I$(top_build_dir)/lib/include
# libInstRO_la_CXXFLAGS=-I$(top_build_dir)/lib/include

