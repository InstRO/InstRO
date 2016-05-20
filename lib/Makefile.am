AM_CPPFLAGS=-I$(top_srcdir)/lib/include -I$(top_builddir)/lib/include -DPROVIDE_MAKE_UNIQUE

AM_CPPFLAGS+= -Werror=return-type
AM_CXXFLAGS=-std=c++11
AM_CFLAGS=

INSTRO_CORE_CPPFLAGS= -Wall -pedantic

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
./include/instro/Instrumentor.h \
./include/instro/Pass.h \
./include/instro/PassFactory.h \
./include/instro/core/ConstructSet.h \
./include/instro/core/PassManager.h \
./include/instro/core/PassImplementation.h \
./include/instro/core/ChannelConfiguration.h \
./include/instro/core/Singleton.h \
./include/instro/core/ConstructTraitType.h \
./include/instro/tooling/IdentifierProvider.h \
./include/instro/tooling/AnalysisInterface.h \
./include/instro/tooling/ConstructElevator.h \
./include/instro/tooling/ControlFlowGraph.h \
./include/instro/tooling/ExtendedCallGraph.h \
./include/instro/tooling/ConstructTraitInterface.h \
./include/instro/tooling/NamedConstructAccess.h \
./include/instro/tooling/TransformationInterface.h \
./include/instro/pass/selector/ProgramEntrySelector.h \
./include/instro/pass/selector/IdentifierMatcherSelector.h \
./include/instro/pass/selector/CallPathSelector.h \
./include/instro/pass/selector/ConstructTraitSelector.h \
./include/instro/pass/selector/ElevatorSelector.h \
./include/instro/pass/selector/BooleanCompoundSelector.h \
./include/instro/pass/adapter/DefaultInstrumentationAdapter.h \
./include/instro/pass/adapter/ConstructPrinterAdapter.h \
./include/instro/pass/adapter/ConstructHierarchyASTDotGenerator.h \
./include/instro/utility/Logger.h \
./include/instro/utility/CommandlineHandling.h \
./include/instro/utility/MemoryManagement.h \
./include/instro/utility/Environment.h

CORE_SFILES= \
./src/Pass.cpp \
./src/PassFactory.cpp \
./src/core/ConstructSet.cpp \
./src/core/PassImplementation.cpp \
./src/core/SimplePassManager.cpp \
./src/core/Singleton.cpp \
./src/core/ConstructTraitType.cpp \
./src/tooling/NamedConstructAccess.cpp \
./src/tooling/ConstructElevator.cpp \
./src/tooling/ExtendedCallGraph.cpp \
./src/tooling/ControlFlowGraph.cpp \
./src/tooling/IdentifierProvider.cpp \
./src/pass/selector/ProgramEntrySelector.cpp \
./src/pass/selector/IdentifierMatcherSelector.cpp \
./src/pass/selector/CallPathSelector.cpp \
./src/pass/selector/ConstructTraitSelector.cpp \
./src/pass/selector/ElevatorSelector.cpp \
./src/pass/selector/BooleanCompoundSelector.cpp  \
./src/pass/adapter/DefaultInstrumentationAdapter.cpp  \
./src/pass/adapter/ConstructPrinterAdapter.cpp \
./src/utility/Logger.cpp \
./src/utility/Environment.cpp \
./src/utility/CommandlineHandling.cpp

CLANG_HFILES= \
./include/instro/clang/pass/selector/ClangBlackWhitelistSelector.h \
./include/instro/clang/pass/adapter/ClangCygProfileAdapter.h \
./include/instro/clang/pass/adapter/ClangMangledNameOutputAdapter.h \
./include/instro/clang/ClangInstrumentor.h \
./include/instro/clang/ClangPassFactory.h \
./include/instro/clang/core/ClangPassImplBase.h \
./include/instro/clang/core/ClangConsumerFactory.h \
./include/instro/clang/core/ClangPassExecuter.h \
./include/instro/clang/core/ClangConstruct.h \
./include/instro/clang/support/InstROASTConsumer.h \
./include/instro/clang/tooling/ClangAnalysisInterface.h \
./include/instro/clang/tooling/ClangConstructTraitInterface.h \
./include/instro/support/BWLFileReader.h

CLANG_SFILES= \
./src/clang/pass/selector/ClangBlackWhitelistSelector.cpp \
./src/clang/pass/adapter/ClangCygProfileAdapter.cpp \
./src/clang/pass/adapter/ClangMangledNameOutputAdapter.cpp \
./src/clang/ClangInstrumentor.cpp \
./src/clang/core/ClangConstruct.cpp \
./src/clang/core/ClangPassFactory.cpp \
./src/clang/support/ClangConsumerFactory.cpp \
./src/clang/support/InstROASTConsumer.cpp \
./src/clang/tooling/ClangConstructTraitInterface.cpp \
./src/support/BWLFileReader.cpp

ROSE_HFILES= \
./include/instro/rose/RoseInstrumentor.h  \
./include/instro/rose/RosePassFactory.h  \
./include/instro/rose/core/RoseConstructSet.h  \
./include/instro/rose/core/RosePass.h  \
./include/instro/rose/core/RoseLegacySelector.h \
./include/instro/rose/tooling/RoseAnalysisInterface.h  \
./include/instro/rose/tooling/RoseConstructElevator.h \
./include/instro/rose/tooling/RoseControlFlowGraph.h \
./include/instro/rose/tooling/RoseExtendedCallGraph.h \
./include/instro/rose/tooling/RoseConstructTraitInterface.h \
./include/instro/rose/tooling/RoseNamedConstructAccess.h \
./include/instro/rose/tooling/RoseTransformationInterface.h \
./include/instro/rose/utility/FunctionRenamer.h \
./include/instro/rose/utility/ScopeStackGuard.hpp \
./include/instro/rose/utility/ASTHelper.h \
./include/instro/rose/utility/ASTTransformer.h \
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
./include/instro/rose/pass/adapter/RoseScorepRegionInstrumentationAdapter.h \
./include/instro/rose/pass/adapter/support/RoseCodeWrapper.h \
./include/instro/rose/pass/adapter/support/RoseScorepCodeWrapper.h \
./include/instro/rose/pass/adapter/support/RosePostOrderInstrumentationAdapter.h \
./include/instro/rose/pass/transformer/RoseUniqueCallpathTransformer.h \
./include/instro/rose/pass/transformer/RoseFunctionWrapper.h

ROSE_SFILES= \
./src/rose/core/RoseConstructSet.cpp \
./src/rose/core/RosePassFactory.cpp \
./src/rose/tooling/RoseConstructElevator.cpp \
./src/rose/tooling/RoseNamedConstructAccess.cpp \
./src/rose/tooling/RoseConstructTraitInterface.cpp \
./src/rose/utility/FunctionRenamer.cpp \
./src/rose/utility/ASTHelper.cpp \
./src/rose/utility/ASTTransformer.cpp \
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
./src/rose/pass/adapter/RoseScorepRegionInstrumentationAdapter.cpp \
./src/rose/pass/adapter/support/RoseCodeWrapper.cpp \
./src/rose/pass/adapter/support/RoseScorepCodeWrapper.cpp \
./src/rose/pass/adapter/support/RosePostOrderInstrumentationAdapter.cpp \
./src/rose/pass/transformer/RoseUniqueCallpathTransformer.cpp \
./src/rose/pass/transformer/RoseFunctionWrapper.cpp


if OPT_RAPIDJSON
CORE_HFILES += ./include/instro/utility/ConfigurationLoader.h
CORE_SFILES += ./src/support/ConfigurationLoader.cpp
ROSE_SFILES += ./src/rose/utility/RoseConfigurationPassRegistry.cpp
CLANG_SFILES += ./src/clang/utility/ClangConfigurationPassRegistry.cpp
endif



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

libInstRO_core_la_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@ $(INSTRO_CORE_CPPFLAGS)

libInstRO_rose_la_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@ -I@ROSE_INCLUDEDIR@

libInstRO_clang_la_CPPFLAGS=$(AM_CPPFLAGS) $(CPPFLAGS) -I@BOOST_INCLUDEDIR@ @LLVM_CXXFLAGS_FLAGS@ -I@LLVM_SRC_INCLUDE_PATH@ -I@CLANG_SRC_INCLUDE_PATH@
libInstRO_clang_la_LDFLAGS=@CLANG_LIBS_LIST@ @LLVM_LDFLAGS@


if OPT_RAPIDJSON
libInstRO_core_la_CPPFLAGS += -I@RAPIDJSON_INCLUDEDIR@
libInstRO_clang_la_CPPFLAGS += -I@RAPIDJSON_INCLUDEDIR@
libInstRO_rose_la_CPPFLAGS += -I@RAPIDJSON_INCLUDEDIR@
endif

include_HEADERS=./include/instro.h
nodist_include_HEADERS=./include/instro/config.h

pkginclude_HEADERS=\
	$(CORE_HFILES) 



