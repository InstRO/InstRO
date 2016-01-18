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


ROSEINSTROASTPRINTER_SFILES=\
./src/rose/extra/main.cxx

CORE_HFILES= \
./include/instro.h \
./include/instro/core/ConstructSet.h \
./include/instro/core/Instrumentor.h \
./include/instro/core/Pass.h \
./include/instro/core/PassFactory.h \
./include/instro/core/PassManager.h \
./include/instro/core/PassImplementation.h \
./include/instro/core/Singleton.h \
./include/instro/tooling/IdentifierProvider.h \
./include/instro/tooling/AnalysisInterface.h \
./include/instro/tooling/ConstructElevator.h \
./include/instro/tooling/ControlFlowGraph.h \
./include/instro/tooling/ExtendedCallGraph.h \
./include/instro/tooling/ConstructClassInterface.h \
./include/instro/tooling/NamedConstructAccess.h \
./include/instro/tooling/TransformationInterface.h \
./include/instro/pass/selector/ProgramEntrySelector.h \
./include/instro/pass/selector/IdentifierSelector.h \
./include/instro/pass/selector/CallPathSelector.h \
./include/instro/pass/selector/ConstructClassSelector.h \
./include/instro/pass/selector/ElevatorSelector.h \
./include/instro/pass/selector/BooleanCompoundSelector.h \
./include/instro/pass/adapter/DefaultInstrumentationAdapter.h \
./include/instro/pass/adapter/ConstructPrinterAdapter.h \
./include/instro/pass/adapter/ConstructHierarchyASTDotGenerator.h \
./include/instro/utility/Logger.h \
./include/instro/utility/MemoryManagement.h

CORE_SFILES= \
./src/core/ConstructSet.cpp \
./src/core/Pass.cpp \
./src/core/PassFactory.cpp \
./src/core/PassImplementation.cpp \
./src/core/SimplePassManager.cpp \
./src/core/Singleton.cpp \
./src/tooling/AnalysisInterface.cpp \
./src/tooling/NamedConstructAccess.cpp \
./src/tooling/ConstructElevator.cpp \
./src/tooling/ExtendedCallGraph.cpp \
./src/tooling/ControlFlowGraph.cpp \
./src/tooling/IdentifierProvider.cpp \
./src/pass/selector/ProgramEntrySelector.cpp \
./src/pass/selector/IdentifierSelector.cpp \
./src/pass/selector/CallPathSelector.cpp \
./src/pass/selector/ConstructClassSelector.cpp \
./src/pass/selector/ElevatorSelector.cpp \
./src/pass/selector/BooleanCompoundSelector.cpp  \
./src/pass/adapter/DefaultInstrumentationAdapter.cpp  \
./src/pass/adapter/ConstructPrinterAdapter.cpp \
./src/utility/Logger.cpp

CLANG_HFILES= \
./include/instro/clang/adapter/CygProfileAdapter.h \
./include/instro/clang/adapter/LLVMInputAdapter.h \
./include/instro/clang/ClangInstrumentor.h \
./include/instro/clang/core/ClangPassImplBase.h \
./include/instro/clang/core/ClangConsumerFactory.h \
./include/instro/clang/core/ClangPassExecuter.h \
./include/instro/clang/core/ClangConstruct.h \
./include/instro/clang/core/ClangPassFactory.h \
./include/instro/clang/core/Pass.h \
./include/instro/clang/selector/BlackWhitelistSelector.h \
./include/instro/clang/selector/FunctionDefinitionSelector.h \
./include/instro/clang/support/InstROASTConsumer.h \
./include/instro/clang/tooling/ClangAnalysisInterface.h \
./include/instro/clang/tooling/ClangConstructClassInterface.h \
./include/instro/support/BWLFileReader.h

CLANG_SFILES= \
./src/clang/adapter/CygProfileAdapter.cpp \
./src/clang/adapter/LLVMInputAdapter.cpp \
./src/clang/ClangInstrumentor.cpp \
./src/clang/core/ClangConstruct.cpp \
./src/clang/core/ClangPassFactory.cpp \
./src/clang/selector/BlackWhitelistSelector.cpp \
./src/clang/selector/FunctionDefinitionSelector.cpp \
./src/clang/support/ClangConsumerFactory.cpp \
./src/clang/support/InstROASTConsumer.cpp \
./src/clang/tooling/ClangConstructClassInterface.cpp \
./src/support/BWLFileReader.cpp

ROSE_HFILES= \
./include/instro/rose/RoseInstrumentor.h  \
./include/instro/rose/RosePassFactory.h  \
./include/instro/rose/core/RoseConstructSet.h  \
./include/instro/rose/core/RosePass.h  \
./include/instro/rose/core/RoseLegacySelector.h \
./include/instro/rose/pass/transformer/RoseUniqueCallpathTransformer.h \
./include/instro/rose/tooling/RoseAnalysisInterface.h  \
./include/instro/rose/tooling/RoseConstructElevator.h \
./include/instro/rose/tooling/RoseControlFlowGraph.h \
./include/instro/rose/tooling/RoseExtendedCallGraph.h \
./include/instro/rose/tooling/RoseConstructClassInterface.h \
./include/instro/rose/tooling/RoseNamedConstructAccess.h \
./include/instro/rose/tooling/RoseTransformationInterface.h \
./include/instro/rose/utility/FunctionRenamer.h \
./include/instro/rose/utility/ScopeStackGuard.hpp \
./include/instro/rose/utility/ASTHelper.h \
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
./include/instro/rose/pass/adapter/RoseDefaultInstrumentationAdapter.h \
./include/instro/rose/pass/adapter/support/RoseCodeWrapper.h \
./include/instro/rose/pass/transformer/RoseUniqueCallpathTransformer.h \
./include/instro/rose/pass/transformer/RoseFunctionWrapper.h

ROSE_SFILES= \
./src/rose/core/RoseConstructSet.cpp \
./src/rose/core/RosePassFactory.cpp \
./src/rose/tooling/RoseConstructElevator.cpp \
./src/rose/tooling/RoseNamedConstructAccess.cpp \
./src/rose/tooling/RoseConstructClassInterface.cpp \
./src/rose/utility/FunctionRenamer.cpp \
./src/rose/utility/ASTHelper.cpp \
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
./src/rose/pass/adapter/RoseDefaultInstrumentationAdapter.cpp \
./src/rose/pass/adapter/support/RoseCodeWrapper.cpp \
./src/rose/pass/transformer/RoseUniqueCallpathTransformer.cpp \
./src/rose/pass/transformer/RoseFunctionWrapper.cpp


if HAVERAPIDJSON
CORE_HFILES += ./include/instro/utility/ConfigurationLoader.h
CORE_SFILES += ./src/support/ConfigurationLoader.cpp
ROSE_SFILES += ./src/rose/utility/RoseConfigurationPassRegistry.cpp
CLANG_SFILES += ./src/clang/utility/ClangConfigurationPassRegistry.cpp
endif


if USELIBTOOL
# LIBTOOL RULE FOR LIBINSTRO
lib_LTLIBRARIES= libInstRO_core.la

if USECLANG
lib_LTLIBRARIES += libInstRO_clang.la
endif

if USEROSE
lib_LTLIBRARIES += libInstRO_rose.la
endif


libInstRO_core_la_SOURCES= \
	$(CORE_SFILES) \
	$(CORE_HFILES)

libInstRO_rose_la_SOURCES= \
	$(CORE_SFILES) \
	$(CORE_HFILES) \
	$(ROSE_SFILES) \
	$(ROSE_HFILES) 

libInstRO_clang_la_SOURCES= \
	$(CLANG_SFILES) \
	$(CLANG_HFILES)

libInstRO_core_la_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

libInstRO_rose_la_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@ -I@ROSE_INCLUDEDIR@

if HAVERAPIDJSON
libInstRO_core_la_CPPFLAGS += -I@RAPIDJSON_INCLUDEDIR@
libInstRO_clang_la_CPPFLAGS = -I@RAPIDJSON_INCLUDEDIR@
libInstRO_rose_la_CPPFLAGS += -I@RAPIDJSON_INCLUDEDIR@
endif

else #USELIBTOOL

# STATIC LIBRARY FOR LIBINSTRO
lib_LIBRARIES= libInstRO_core.a 

if USECLANG
lib_LIBRARIES += libInstRO_clang.a
endif

if USEROSE
lib_LIBRARIES += libInstRO_rose.a
endif


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

libInstRO_clang_a_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) @LLVM_CXXFLAGS_FLAGS@ -I@LLVM_SRC_INCLUDE_PATH@ -I@CLANG_SRC_INCLUDE_PATH@ -I@BOOST_INCLUDEDIR@

# We use LDFLAGS for this list according to https://www.gnu.org/software/automake/manual/html_node/Libtool-Flags.html#Libtool-Flags
libInstRO_clang_a_LDFLAGS=@CLANG_LIBS_LIST@ @LLVM_LDFLAGS@

libInstRO_core_a_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@

if HAVERAPIDJSON
libInstRO_core_a_CPPFLAGS += -I@RAPIDJSON_INCLUDEDIR@
libInstRO_clang_a_CPPFLAGS += -I@RAPIDJSON_INCLUDEDIR@
libInstRO_rose_a_CPPFLAGS += -I@RAPIDJSON_INCLUDEDIR@
endif

endif #USELIBTOOL

include_HEADERS=./include/instro.h
nodist_include_HEADERS=./include/instro/config.h

pkginclude_HEADERS=\
	$(CORE_HFILES) 



