AM_CPPFLAGS=-I$(top_builddir)/lib/include
AM_CFALGS=-O0

if DEBUG
AM_CFLAGS = -g3 -O0
AM_CXXFLAGS = -g3 -O0 -std=gnu++0x
else
AM_CFLAGS = -O2
AM_CXXFLAGS = -O2 -std=gnu++0x
endif

#SFILES=\
#./src/main.cxx

#./src/instrumentor.cpp  ./src/common.cpp ./src/utility/functionals.cpp ./src/utility/matcher.cpp ./src/utility/callgraphmanager.cpp ./src/utility/asthelper.cpp ./src/marker.cpp \
#./src/adapterbuilder/CygProfileFunctionAdapterBuilder.cpp ./src/adapterbuilder/VampirTraceAdapterBuilder.cpp \
#./src/adapterbuilder/FunctionPrinterAdapterBuilder.cpp ./src/adapterbuilder/OpenMPAdapterBuilder.cpp \
#./src/adapterbuilder/BackendInstrumentationAdapterBuilder.cpp ./src/adapterbuilder/GenericFunctionAdapterBuilder.cpp \
#./src/adapterbuilder/ShadowStackFunctionAdapterBuilder.cpp ./src/adapterbuilder/CallSiteAdapterBuilder.cpp \
#./src/contrib/geimeradapter.cpp ./src/contrib/geimerparser.cpp ./src/contrib/geimerselector.cpp \
#./src/selectors/nodetypebasedselectors.cpp ./src/selectors/namematchingselector.cpp ./src/selectors/blackwhitelistselector.cpp ./src/selectors/openmpselector.cpp \
#./src/selectors/calldepthselector.cpp ./src/selectors/selectorbasedselector.cpp ./src/selectors/callpathselector.cpp ./src/selectors/sourcefileselector.cpp ./src/selectors/profilebasedselector.cpp \
#./src/selectors/statementcountselector.cpp  ./src/selectors/flopcountselector.cpp ./src/selectors/fileoriginselector.cpp \
#./src/selectors/callfrequencyheuristicselector.cpp \
#./src/selectors/compoundselector.cpp \
#./src/selectors/CubeEdgeBasedSelector.cpp \
#./src/utility/instrobuilder.cpp ./src/utility/functionidentification.cpp ./src/utility/overheadestimator.cpp \
#./src/exception/instroexception.cpp \
#./src/selectors/cashesselector.cpp

HFILES=\
./include/instro.h \
./include/instro/interface.h \
./include/instro/agnosticSupport.h \
./include/instro/roseInstRO.h \
./include/instro/llvmInstRO.h

#./include/instro.h \
#./include/instro/instrumentor.h ./include/instro/common.h ./include/instro/utility/functionals.h ./include/instro/utility/matcher.h  ./include/instro/utility/callgraphmanager.h ./include/instro/utility/asthelper.h \
#./include/instro/utility/marker.h \
#./include/instro/adapterbuilder/CygProfileFunctionAdapterBuilder.h ./include/instro/adapterbuilder/VampirTraceAdapterBuilder.h ./include/instro/adapterbuilder/FunctionPrinterAdapterBuilder.h \
#./include/instro/adapterbuilder/OpenMPAdapterBuilder.h ./include/instro/adapterbuilder/BackendInstrumentationAdapterBuilder.h ./include/instro/adapterbuilder/GenericFunctionAdapterBuilder.h \
#./include/instro/adapterbuilder/ShadowStackFunctionAdapterBuilder.h ./include/instro/adapterbuilder/CallSiteAdapterBuilder.h \
#./include/instro/contrib/geimeradapter.h ./include/instro/contrib/geimerparser.h ./include/instro/contrib/geimerselector.h \
#./include/instro/selectors/nodetypebasedselectors.h ./include/instro/selectors/namematchingselector.h ./include/instro/selectors/blackwhitelistselector.h ./include/instro/selectors/openmpselector.h \
#./include/instro/selectors/calldepthselector.h ./include/instro/selectors/selectorbasedselector.h ./include/instro/selectors/callpathselector.h ./include/instro/selectors/sourcefileselector.h \
#./include/instro/selectors/profilebasedselector.h \
#./include/instro/selectors/statementcountselector.h ./include/instro/selectors/flopcountselector.h ./include/instro/selectors/fileoriginselector.h \
#./include/instro/selectors/callfrequencyheuristicselector.h \
#./include/instro/selectors/compoundselector.h \
#./include/instro/selectors/CubeEdgeBasedSelector.h \
#./include/instro/utility/instrobuilder.h ./include/instro/utility/functionidentification.h ./include/instro/utility/overheadestimator.h \
#./include/instro/selectors/cashesselector.h \
#./include/instro/exception/instroexception.h

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

libInstRO_a_CPPFLAGS=-I$(srcdir)/include -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@

endif

include_HEADERS=./include/instro.h

pkginclude_HEADERS=\
$(HFILES)


## libInstRO_la_DEPENDENCIES = @ROSE@
# Add Rose as an dependency
## libInstRO_la_LIBADD= @ROSE@
libInstRO_la_CPPFLAGS=-I$(srcdir)/include -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@
## libInstRO_la_CFLAGS=-I@ROSE_INCLUDEDIR@
## libInstRO_la_CXXFLAGS=-I@ROSE_INCLUDEDIR@
libInstRO_la_LDFLAGS=-L@ROSE_LIBDIR@
# libInstRO_la_CFLAGS=-I$(top_build_dir)/lib/include
# libInstRO_la_CXXFLAGS=-I$(top_build_dir)/lib/include

