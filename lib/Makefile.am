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


ROSEINSTROASTPRINTER_SFILES=\
./src/rose/extra/main.cxx

CLANG_SFILES= \
./src/clang/adapter/CygProfileAdapter.cpp \
./src/clang/adapter/LLVMInputAdapter.cpp \
./src/clang/ClangInstrumentor.cpp \
./src/clang/core/ClangPassExecuter.cpp \
./src/clang/core/ConstructSet.cpp \
./src/clang/core/PassFactory.cpp \
./src/clang/selector/BlackWhitelistSelector.cpp \
./src/clang/selector/FunctionDefinitionSelector.cpp \
./src/clang/selector/BooleanCompoundSelector.cpp \
./src/clang/support/ClangConsumerFactory.cpp \
./src/clang/support/InstROASTConsumer.cpp \
./src/support/BWLFileReader.cpp

EXAMPLE_SFILES= \
./src/example/ExampleConstructPrinter.cpp \
./src/example/ExampleConstructSet.cpp \
./src/example/ExampleInstrumentor.cpp \
./src/example/ExampleNamedConstructAccess.cpp \
./src/example/NameBasedSelector.cpp \
./src/example/CompoundSelector.cpp

#./src/clang/Instrumentor.cpp 

CORE_HFILES= \
./include/instro.h \
./include/instro/core/ConstructSet.h \
./include/instro/core/Factory.h \
./include/instro/core/Instrumentor.h \
./include/instro/core/Pass.h \
./include/instro/core/PassFactory.h \
./include/instro/core/PassManager.h \
./include/instro/core/PassImplementation.h \
./include/instro/core/Singleton.h \
./include/instro/tooling/AnalysisInterface.h \
./include/instro/tooling/ConstructElevator.h \
./include/instro/tooling/ControlFlowGraph.h \
./include/instro/tooling/ExtendedCallGraph.h \
./include/instro/tooling/GrammarInterface.h \
./include/instro/tooling/NamedConstructAccess.h \
./include/instro/tooling/TransformationInterface.h \
./include/instro/pass/selector/ProgramEntrySelector.h \
./include/instro/pass/selector/IdentifyerSelector.h \
./include/instro/pass/selector/CallPathSelector.h \
./include/instro/pass/selector/ConstructClassSelector.h \
./include/instro/pass/selector/ElevatorSelector.h

CORE_SFILES= \
./src/core/ConstructSet.cpp \
./src/core/Pass.cpp \
./src/core/PassImplemention.cpp \
./src/core/SimplePassManager.cpp \
./src/core/Singleton.cpp \
./src/core/Helper.cpp \
./src/tooling/AnalysisInterface.cpp \
./src/tooling/NamedConstructAccess.cpp \
./src/tooling/ConstructElevator.cpp \
./src/tooling/ExtendedCallGraph.cpp \
./src/tooling/ControlFlowGraph.cpp \
./src/pass/selector/ProgramEntrySelector.cpp \
./src/pass/selector/IdentifyerSelector.cpp \
./src/pass/selector/CallPathSelector.cpp \
./src/pass/selector/ConstructClassSelector.cpp \
./src/pass/selector/ElevatorSelector.cpp

CLANG_HFILES= \
./include/instro/clang/adapter/CygProfileAdapter.h \
./include/instro/clang/adapter/LLVMInputAdapter.h \
./include/instro/clang/ClangInstrumentor.h \
./include/instro/clang/core/ClangPassImplBase.h \
./include/instro/clang/core/ClangConsumerFactory.h \
./include/instro/clang/core/ClangPassExecuter.h \
./include/instro/clang/core/ConstructSet.h \
./include/instro/clang/core/PassFactory.h \
./include/instro/clang/core/Pass.h \
./include/instro/clang/selector/BlackWhitelistSelector.h \
./include/instro/clang/selector/FunctionDefinitionSelector.h \
./include/instro/clang/selector/BooleanCompoundSelector.h \
./include/instro/clang/support/InstROASTConsumer.h \
./include/instro/support/BWLFileReader.h

ROSE_HFILES= \
./include/instro/rose/RoseInstrumentor.h  \
./include/instro/rose/RosePassFactory.h  \
./include/instro/rose/core/RoseConstructSet.h  \
./include/instro/rose/core/RosePass.h  \
./include/instro/rose/core/RoseLegacySelector.h \
./include/instro/rose/pass/adapter/RoseConstructPrinter.h \
./include/instro/rose/pass/selector/NameBasedSelector.h \
./include/instro/rose/pass/selector/CompoundSelector.h \
./include/instro/rose/pass/transformer/UniqueCallpathTransformer.h \
./include/instro/rose/tooling/RoseAnalysisInterface.h  \
./include/instro/rose/tooling/RoseConstructElevator.h \
./include/instro/rose/tooling/RoseControlFlowGraph.h \
./include/instro/rose/tooling/RoseExtendedCallGraph.h \
./include/instro/rose/tooling/RoseGrammarInterface.h \
./include/instro/rose/tooling/RoseNamedConstructAccess.h \
./include/instro/rose/tooling/RoseTransformationInterface.h \
./include/instro/rose/pass/adapter/ConstructHierarchyASTDotGenerator.h \
./include/instro/rose/utility/FunctionRenamer.h \
./include/instro/rose/pass/adapter/RoseStrategyBasedAdapter.h \
./include/instro/rose/pass/adapter/StrategyBasedAdapterSupport/CodeInsertionHelper.h \
./include/instro/rose/pass/adapter/StrategyBasedAdapterSupport/CodeInsertionPointSelector.h \
./include/instro/rose/pass/adapter/StrategyBasedAdapterSupport/GenericInstrumentationStrategy.h \
./include/instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePFunctionScopeStrategy.h \
./include/instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePInitializer.h \
./include/instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePLoopIterationStrategy.h \
./include/instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePStatementWrapperStrategy.h \
./include/instro/rose/pass/adapter/StrategyBasedAdapterSupport/ToolInitializer.h \
./include/instro/rose/pass/adapter/StrategyBasedAdapterSupport/instrobuilder.h \
./include/instro/rose/pass/transformer/UniqueCallpathTransformer.h

ROSE_SFILES= \
./src/rose/core/RoseConstructSet.cpp   \
./src/rose/core/RosePassFactory.cpp \
./src/rose/pass/adapter/RoseConstructPrinter.cpp \
./src/rose/pass/selector/CompoundSelector.cpp  \
./src/rose/pass/selector/NameBasedSelector.cpp  \
./src/rose/tooling/RoseConstructElevator.cpp \
./src/rose/tooling/RoseNamedConstructAccess.cpp \
./src/rose/tooling/RoseGrammarInterface.cpp \
./src/rose/pass/transformer/UniqueCallpathTransformer.cpp \
./src/rose/utility/FunctionRenamer.cpp \
./src/rose/pass/adapter/StrategyBasedAdapterSupport/CodeInsertionHelper.cpp \
./src/rose/pass/adapter/RoseStrategyBasedAdapter.cpp \
./src/rose/pass/adapter/StrategyBasedAdapterSupport/CodeInsertionPointSelector.cpp \
./src/rose/pass/adapter/StrategyBasedAdapterSupport/GenericInstrumentationStrategy.cpp \
./src/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePFunctionScopeStrategy.cpp \
./src/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePInitializer.cpp \
./src/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePLoopIterationStrategy.cpp \
./src/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePStatementWrapperStrategy.cpp \
./src/rose/pass/adapter/StrategyBasedAdapterSupport/ToolInitializer.cpp \
./src/rose/pass/adapter/StrategyBasedAdapterSupport/instrobuilder.cpp \
./src/rose/pass/transformer/UniqueCallpathTransformer.cpp

EXAMPLE_HFILES= \
./include/instro/example/ExampleConstructSet.h \
./include/instro/example/ExampleInstrumentor.h \
./include/instro/example/ExamplePassFactory.h \
./include/instro/example/ExamplePass.h \
./include/instro/example/ExampleAnalysisInterface.h \
./include/instro/example/ExampleConstructPrinter.h  \
./include/instro/example/ExampleNamedConstructAccess.h \
./include/instro/example/NameBasedSelector.h


if HAVERAPIDJSON
CORE_HFILES += ./include/instro/utility/ConfigurationLoader.h
CORE_SFILES += ./src/support/ConfigurationLoader.cpp
ROSE_SFILES += ./src/rose/utility/RoseConfigurationPassRegistry.cpp
CLANG_SFILES += ./src/clang/utility/ClangConfigurationPassRegistry.cpp
endif


if USELIBTOOL
# LIBTOOL RULE FOR LIBINSTRO
lib_LTLIBRARIES= libInstRO_core.la libInstRO_example.la
#lib_LTLIBRARIES= libInstRO_core.la libInstRO_example.la libInstRO_rose.la
bin_PROGRAMS=ExampleInstrumentor

if USECLANG
lib_LTLIBRARIES += libInstRO_clang.la
endif

if USEROSE
lib_LTLIBRARIES += libInstRO_rose.la
bin_PROGRAMS += RoseInstrumentor ROSEInstROASTPrinter
endif

if HAVERAPIDJSON
bin_PROGRAMS += ConfigInstrumentor
endif


libInstRO_core_la_SOURCES= \
	$(CORE_SFILES) \
	$(CORE_HFILES)

libInstRO_example_la_SOURCES= \
	$(CORE_SFILES) \
	$(CORE_HFILES) \
	$(EXAMPLE_SFILES) \
	$(EXAMPLE_HFILES) 

libInstRO_rose_la_SOURCES= \
	$(CORE_SFILES) \
	$(CORE_HFILES) \
	$(ROSE_SFILES) \
	$(ROSE_HFILES) 

libInstRO_clang_la_SOURCES= \
	$(CLANG_SFILES) \
	$(CLANG_HFILES)

#libInstRO_core_la_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@CLANG_INCLUDE_PATH@ -I@LLVM_INCLUDE_PATH@ -I@BOOST_INCLUDEDIR@
libInstRO_core_la_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

libInstRO_example_la_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

libInstRO_rose_la_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@ -I@ROSE_INCLUDEDIR@

RoseInstrumentor_SOURCES = \
	./src/roseInstrumentor.cpp
RoseInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@
RoseInstrumentor_LDADD=libInstRO_rose.la @ROSE_LINK_LIBRARIES@

ExampleInstrumentor_SOURCES = \
	./src/example_main.cpp

ROSEInstROASTPrinter_SOURCES = \ 
	./src/rose/extra/instro_rose_astprinter/main.cxx
ROSEInstROASTPrinter_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@
ROSEInstROASTPrinter_LDADD=libInstRO_rose.la @ROSE_LINK_LIBRARIES@

ExampleInstrumentor_LDADD=libInstRO_example.la
# ExampleInstrumentor_LDFLAGS=
ExampleInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

ConfigInstrumentor_SOURCES=./src/config_main.cpp
ConfigInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@ -I@ROSE_INCLUDEDIR@ -I@RAPIDJSON_INCLUDEDIR@
ConfigInstrumentor_LDADD=libInstRO_rose.la @ROSE_LINK_LIBRARIES@

if HAVERAPIDJSON
libInstRO_core_la_CPPFLAGS += -I@RAPIDJSON_INCLUDEDIR@
libInstRO_example_la_CPPFLAGS += -I@RAPIDJSON_INCLUDEDIR@
libInstRO_rose_la_CPPFLAGS += -I@RAPIDJSON_INCLUDEDIR@
endif


#ClangInstrumentor_SOURCES = \
#	./src/clang/Instrumentor.cpp

#ClangInstrumentor_LDADD=libInstRO_core.a libInstRO_clang.a @LLVM_LDFLAGS@ @CLANG_LIBS_LIST@ @CLANG_LIBS_LIST@
#ClangInstrumentor_LDFLAGS= @LLVM_LDFLAGS@ @CLANG_LIBS_LIST@ @CLANG_LIBS_LIST@
#ClangInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@CLANG_SRC_INCLUDE_PATH@ -I@LLVM_SRC_INCLUDE_PATH@  @LLVM_CXXFLAGS_FLAGS@ -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@
#ClangInstrumentor_LIBTOOLFLAGS=--preserve-dup-deps



else #USELIBTOOL

# STATIC LIBRARY FOR LIBINSTRO
lib_LIBRARIES= libInstRO_core.a libInstRO_example.a

# bin_PROGRAMS=ClangInstrumentor ExampleInstrumentor RoseInstrumentor
bin_PROGRAMS=ExampleInstrumentor 

if USECLANG
lib_LIBRARIES += libInstRO_clang.a
endif

if USEROSE
lib_LIBRARIES += libInstRO_rose.a
bin_PROGRAMS += RoseInstrumentor ROSEInstROASTPrinter
endif

if HAVERAPIDJSON
bin_PROGRAMS += ConfigInstrumentor
endif


#ClangInstrumentor_SOURCES = \
	./src/clang/Instrumentor.cpp

#ClangInstrumentor_LDADD=libInstRO_core.a libInstRO_clang.a @CLANG_LIBS_LIST@ @LLVM_LDFLAGS@
#ClangInstrumentor_LDFLAGS=@LLVM_LDFLAGS@ @CLANG_LIBS_LIST@
#ClangInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@CLANG_SRC_INCLUDE_PATH@ -I@LLVM_SRC_INCLUDE_PATH@ @LLVM_CXXFLAGS_FLAGS@ -I@BOOST_INCLUDEDIR@

RoseInstrumentor_SOURCES = \
	./src/roseInstrumentor.cpp

RoseInstrumentor_LDADD=libInstRO_core.a libInstRO_rose.a
RoseInstrumentor_LDFLAGS=-L@ROSE_LIBDIR@ @ROSE_LINK_LIBRARIES@
RoseInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@

ROSEInstROASTPrinter_SOURCES = \ 
	./src/rose/extra/instro_rose_astprinter/main.cxx
ROSEInstROASTPrinter_LDADD=libInstRO_core.a libInstRO_rose.a
ROSEInstROASTPrinter_LDFLAGS=-L@ROSE_LIBDIR@ @ROSE_LINK_LIBRARIES@
ROSEInstROASTPrinter_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@ROSE_INCLUDEDIR@ -I@BOOST_INCLUDEDIR@

ExampleInstrumentor_SOURCES = \
	./src/example_main.cpp

ExampleInstrumentor_LDADD= libInstRO_example.a
ExampleInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

ConfigInstrumentor_SOURCES =./src/config_main.cpp
ConfigInstrumentor_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@ -I@ROSE_INCLUDEDIR@ -I@RAPIDJSON_INCLUDEDIR@
ConfigInstrumentor_LDADD=libInstRO_core.a libInstRO_rose.a
ConfigInstrumentor_LDFLAGS=-L@ROSE_LIBDIR@ @ROSE_LINK_LIBRARIES@


libInstRO_example_a_SOURCES= \
	$(CORE_SFILES) \
	$(CORE_HFILES) \
	$(EXAMPLE_SFILES) \
	$(EXAMPLE_HFILES) 
libInstRO_example_a_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

libInstRO_rose_a_SOURCES= \
	$(CORE_SFILES) \
	$(CORE_HFILES) \
	$(ROSE_SFILES) \
	$(ROSE_HFILES) 
libInstRO_rose_a_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@ -I@ROSE_INCLUDEDIR@

libInstRO_core_a_SOURCES= \
	$(CORE_SFILES) \
	$(CORE_HFILES) 

libInstRO_clang_a_SOURCES= \
	$(CORE_SFILES) \
	$(CORE_HFILES) \
	$(CLANG_SFILES) \
	$(CLANG_HFILES)

libInstRO_clang_a_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@LLVM_SRC_INCLUDE_PATH@ @LLVM_CXXFLAGS_FLAGS@ -I@CLANG_SRC_INCLUDE_PATH@ -I@BOOST_INCLUDEDIR@

# We use LDFLAGS for this list according to https://www.gnu.org/software/automake/manual/html_node/Libtool-Flags.html#Libtool-Flags
libInstRO_clang_a_LDFLAGS=@CLANG_LIBS_LIST@ @LLVM_LDFLAGS@

libInstRO_core_a_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

## Manually crafted Clang stuff!
# FIXME Figure out how this can be done using actually autotools
ClangInstRO:
	$(CXX) --verbose $(LLVM_CXXFLAGS_FLAGS) -I$(LLVM_SRC_INCLUDE_PATH) -I$(CLANG_SRC_INCLUDE_PATH) -I$(BOOST_INCLUDEDIR) $(AM_CPPFLAGS) $(top_srcdir)/lib/src/clang/Instrumentor.cpp libInstRO_clang.a libInstRO_core.a $(CLANG_LIBS_LIST) $(LLVM_LDFLAGS) -o $@


if HAVERAPIDJSON
libInstRO_core_a_CPPFLAGS += -I@RAPIDJSON_INCLUDEDIR@
libInstRO_example_a_CPPFLAGS += -I@RAPIDJSON_INCLUDEDIR@
libInstRO_rose_a_CPPFLAGS += -I@RAPIDJSON_INCLUDEDIR@
endif

endif #USELIBTOOL

include_HEADERS=./include/instro.h
nodist_include_HEADERS=./include/instro/config.h

pkginclude_HEADERS=\
	$(CORE_HFILES) \
	$(EXAMPLE_HFILES)



