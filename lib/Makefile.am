AM_CPPFLAGS=-I$(top_srcdir)/lib/include
AM_CFLAGS=-O0

if DEBUG
AM_CFLAGS = -g3 -O0
AM_CPPFLAGS += -g3 -O0 -std=gnu++0x
else
AM_CFLAGS = -O2
AM_CPPFLAGS += -O2 -std=gnu++0x
endif

CORE_SFILES= \
./src/core/ConstructSet.cpp \
./src/core/ConstructSetManagement.cpp \
./src/core/SimplePassManager.cpp \
./src/core/Pass.cpp

CLANG_SFILES= \
./src/clang/adapter/CygProfileAdapter.cpp \
./src/clang/ClangInstrumentor.cpp \
./src/clang/core/ClangPassExecuter.cpp \
./src/clang/core/ConstructSet.cpp \
./src/clang/core/PassFactory.cpp \
./src/clang/selector/BlackWhitelistSelector.cpp \
./src/clang/selector/FunctionDefinitionSelector.cpp \
./src/clang/support/ClangConsumerFactory.cpp \
./src/clang/support/InstROASTConsumer.cpp

#./src/clang/Instrumentor.cpp 

CORE_HFILES= \
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
./include/instro/core/PassImplementation.h 

CLANG_HFILES= \
./include/instro/clang/adapter/CygProfileAdapter.h \
./include/instro/clang/ClangInstrumentor.h \
./include/instro/clang/core/ClangAdapterPass.h \
./include/instro/clang/core/ClangConsumerFactory.h \
./include/instro/clang/core/ClangPassExecuter.h \
./include/instro/clang/core/ConstructSet.h \
./include/instro/clang/core/PassFactory.h \
./include/instro/clang/core/Pass.h \
./include/instro/clang/selector/BlackWhitelistSelector.h \
./include/instro/clang/selector/FunctionDefinitionSelector.h \
./include/instro/clang/support/InstROASTConsumer.h
#./include/instro/agnosticSupport.h \
#./include/instro/misc/VisualizingManager.h \
#./include/instro/misc/VisualizingPassManager.h \
#./include/instro/rose/RoseInstrumentor.h \
#./include/instro/rose/RosePass.h \
#./include/instro/rose/RosePassFactory.h \
#./include/instro/rose/contrib/geimeradapter.h \
#./include/instro/rose/contrib/geimerparser.h \
#./include/instro/rose/contrib/geimerselector.h \
#./include/instro/rose/selectors/BlackAndWhiteListSelector.h \
#./include/instro/rose/selectors/CompoundSelectors.h \
#./include/instro/rose/adapters/CygProfileAdapter.h \
#./include/instro/rose/adapters/GenericAdapter.h \
#./include/instro/roseInstRO.h \
#./include/instro/llvmInstRO.h

if USELIBTOOL
# LIBTOOL RULE FOR LIBINSTRO
#lib_LTLIBRARIES= libInstRO_core.la libInstRO_clang.la libInstRO.la
lib_LTLIBRARIES= libInstRO_core.la
libInstRO_core_la_SOURCES= \
	$(CORE_SFILES) \
	$(CORE_HFILES)

#libInstRO_clang_la_SOURCES= \
#	$(CLANG_SFILES) \
#	$(CLANG_HFILES)

#libInstRO_la_LIBADD= libInstRO_core.la libInstRO_clang.la
#libInstRO_la_CXXFLAGS= \
#	-I$(srcdir) \
#	-I$(srcdir)/include/instro

## libInstRO_la_DEPENDENCIES = @ROSE@
# Add Rose as an dependency
## libInstRO_la_LIBADD= @ROSE@
libInstRO_core_la_CPPFLAGS=-I$(srcdir)/include -I@CLANG_INCLUDE_PATH@ -I@LLVM_INCLUDE_PATH@ -I@ROSE_INCLUDEDIR@ -I@ROSE_NEW_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@
## libInstRO_la_CFLAGS=-I@ROSE_INCLUDEDIR@
## libInstRO_la_CXXFLAGS=-I@ROSE_INCLUDEDIR@
#libInstRO_la_LDFLAGS=-L@ROSE_LIBDIR@
# libInstRO_la_CFLAGS=-I$(top_build_dir)/lib/include
# libInstRO_la_CXXFLAGS=-I$(top_build_dir)/lib/include

else
# STATIC LIBRARY FOR LIBINSTRO
#lib_LIBRARIES=libInstRO.a
lib_LIBRARIES= libInstRO_core.a 
#libInstRO_clang.a

bin_PROGRAMS=ClangInstrumentor

ClangInstrumentor_SOURCES = \
	./src/clang/Instrumentor.cpp

libInstRO_core_a_SOURCES= \
	$(CORE_SFILES) \
	$(CORE_HFILES) \
	$(CLANG_SFILES) \
	$(CLANG_HFILES)
#libInstRO_clang_a_SOURCES= 

#libInstRO_core_a_LIBADD=-L@CLANG_LIB_DIR@ @CLANG_LIBS_LIST@ @LLVM_LINK_COMMAND@ -lclangToolingCore
#ClangInstrumentor_LDADD= libInstRO_core.a libInstRO_clang.a -L@CLANG_LIB_DIR@ @CLANG_LIBS_LIST@ @LLVM_LINK_COMMAND@
ClangInstrumentor_LDADD= libInstRO_core.a -L@CLANG_LIB_DIR@ @CLANG_LIBS_LIST@ @LLVM_LINK_COMMAND@ -lclang
ClangInstrumentor_LDFLAGS= -L@CLANG_LIB_DIR@ @CLANG_LIBS_LIST@ @LLVM_LINK_COMMAND@ -lclang

#libInstRO_a_SOURCES=\
#$(SFILES) \
#$(HFILES)

libInstRO_core_a_CPPFLAGS=-std=c++11 -I$(top_srcdir)/lib/include @LLVM_INCLUDE_FLAGS@ -I@CLANG_INCLUDE_PATH@ -I@ROSE_INCLUDEDIR@ -I@ROSE_NEW_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@

#libInstRO_clang_a_CPPFLAGS=-std=c++11 -I$(top_srcdir)/lib/include @LLVM_INCLUDE_FLAGS@ -I@CLANG_INCLUDE_PATH@ -I@ROSE_INCLUDEDIR@ -I@ROSE_NEW_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@

ClangInstrumentor_CPPFLAGS= -std=c++11 -I$(top_srcdir)/lib/include @LLVM_INCLUDE_FLAGS@ -I@CLANG_INCLUDE_PATH@ -I@ROSE_INCLUDEDIR@ -I@ROSE_NEW_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@

endif

include_HEADERS=./include/instro.h

pkginclude_HEADERS=\
	$(CORE_HFILES) \
	$(CLANG_HFILES)



