# I definitiley should rename this!
CLANG_BASE_PATH=$(CLANG_BASE)


# Actually all these variables are pointing to the llvm base path
LLVM_SRC_PATH=$(CLANG_BASE_PATH)/repo

LLVM_BUILD_PATH=$(CLANG_BASE_PATH)/build

LLVM_BIN_PATH=$(CLANG_BASE_PATH)/install/bin

CXX=g++


LLVM_CXXFLAGS = `$(LLVM_BIN_PATH)/llvm-config --cxxflags`
LLVM_LDFLAGS = `$(LLVM_BIN_PATH)/llvm-config --ldflags --libs --system-libs`


LLVM_LDFLAGS_NOLIBS = `$(LLVM_BIN_PATH)/llvm-config --ldflags`
PLUGIN_LDFLAGS= -shared

CLANG_INCLUDE := \
								-I$(LLVM_SRC_PATH)/tools/clang/include \
								-I$(LLVM_BUILD_PATH)/tools/clang/include



CLANG_LIBS := \
				-Wl,--start-group \
				-lclangAST \
				-lclangAnalysis \
				-lclangBasic \
				-lclangDriver \
				-lclangEdit \
				-lclangFrontend \
				-lclangFrontendTool \
				-lclangLex \
				-lclangParse \
				-lclangSema \
				-lclangEdit \
				-lclangASTMatchers \
				-lclangRewrite \
				-lclangRewriteFrontend \
				-lclangStaticAnalyzerFrontend \
				-lclangStaticAnalyzerCheckers \
				-lclangStaticAnalyzerCore \
				-lclangSerialization \
				-lclangToolingCore \
				-lclangTooling \
				-Wl,--end-group


#all: clang_howto first_matcher fDefMatcher
all: clang-tool

INCLUDE_FLAGS=-I.
CXXFLAGS+=-O0 -g $(INCLUDE_FLAGS) 
PLUGIN_CXXFLAGS= -fpic


TOP_LEVEL=/home/j_lehr/all_repos/instro-v5/repo
INSTROLLVMINCLUDE=$(TOP_LEVEL)/lib/include/instro/llvm
INSTROCLANGINCLUDE=$(TOP_LEVEL)/lib/include/instro/clang

INSTRO_INCLUDES=-I$(TOP_LEVEL)/lib/include -I$(INSTROLLVMINCLUDE) -I$(INSTROLLVMINCLUDE)/selector -I$(INSTROLLVMINCLUDE)/adapter -I$(INSTROLLVMINCLUDE)/core 

INSTRO_INCLUDES+=-I$(INSTROCLANGINCLUDE) \
								 -I$(INSTROCLANGINCLUDE)/selector \
								 -I$(INSTROCLANGINCLUDE)/adapter \
								 -I$(INSTROCLANGINCLUDE)/core \
								 -I$(BOOST)/include


LLVM_SOURCES=$(TOP_LEVEL)/lib/src/llvm/selector/CashesSelector.cpp \
				$(TOP_LEVEL)/lib/src/llvm/adapter/CygProfileAdapter.cpp \
				$(TOP_LEVEL)/lib/src/llvm/Register.cpp

CLANG_SOURCES= \
				$(TOP_LEVEL)/lib/src/core/SimplePassManager.cpp \
				$(TOP_LEVEL)/lib/src/core/Pass.cpp \
				$(TOP_LEVEL)/lib/src/core/ConstructSetManagement.cpp \
				$(TOP_LEVEL)/lib/src/clang/selector/FunctionDefinitionSelector.cpp \
				$(TOP_LEVEL)/lib/src/clang/selector/BlackWhitelistSelector.cpp \
				$(TOP_LEVEL)/lib/src/clang/adapter/CygProfileAdapter.cpp \
				$(TOP_LEVEL)/lib/src/clang/core/PassFactory.cpp \
				$(TOP_LEVEL)/lib/src/clang/core/ConstructSet.cpp \
				$(TOP_LEVEL)/lib/src/clang/core/ClangPassExecuter.cpp \
				$(TOP_LEVEL)/lib/src/clang/ClangInstrumentor.cpp \
				$(TOP_LEVEL)/lib/src/clang/support/InstROASTConsumer.cpp \
				$(TOP_LEVEL)/lib/src/clang/support/ClangConsumerFactory.cpp \
				$(TOP_LEVEL)/lib/src/support/BWLFileReader.cpp \
				$(TOP_LEVEL)/lib/src/clang/Instrumentor.cpp

LLVM_OBJECTS=$(LLVM_SOURCES:.cpp=.o)
CLANG_OBJECTS=$(CLANG_SOURCES:.cpp=.o)

cashes-pass: $(LLVM_OBJECTS)
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(PLUGIN_LDFLAGS) $(LLVM_OBJECTS) $(CLANG_LIBS) $(LLVM_LDFLAGS) -o cashesPass.so

clang-tool: $(CLANG_OBJECTS)
	$(CXX) $(LLVM_CXXFLAGS) $(CXXFLAGS)  $(CLANG_OBJECTS) $(CLANG_LIBS) $(LLVM_LDFLAGS) -o ClangInstrumentor.out

.cpp.o:
	$(CXX) $(INSTRO_INCLUDES) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(PLUGIN_LDFLAGS) -c $< -o $@
#$(CLANG_LIBS) $(LLVM_LDFLAGS) 

cashes-pass-depr:
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(PLUGIN_LDFLAGS) cashesSelectorPass.cpp $(CLANG_LIBS) $(LLVM_LDFLAGS_NOLIBS) -o b_cashes.so
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(PLUGIN_LDFLAGS) cashesSelector.cpp $(CLANG_LIBS) $(LLVM_LDFLAGS_NOLIBS) -o b_cashesSel.so
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(PLUGIN_LDFLAGS) cygProfileAdapter.cpp $(CLANG_LIBS) $(LLVM_LDFLAGS_NOLIBS) -o b_cygProfileAdapter.so


clean:
	rm -rf *.so
	rm -rf *.o
