# I definitiley should rename this!
CLANG_BASE_PATH=$(CLANG_BASE)


# Actually all these variables are pointing to the llvm base path
LLVM_SRC_PATH=$(CLANG_BASE_PATH)/repo

LLVM_BUILD_PATH=$(CLANG_BASE_PATH)/build

LLVM_BIN_PATH=$(CLANG_BASE_PATH)/install/bin

CXX=g++

INCLUDE_FLAGS=-I.

CXXFLAGS= -O0 -g $(INCLUDE_FLAGS)
PLUGIN_CXXFLAGS= -fpic

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
all: cashes-pass


TOP_LEVEL=/home/j_lehr/all_repos/instro-v5/repo
MYINCLUDE=$(TOP_LEVEL)/lib/include/instro/llvm

INSTRO_INCLUDES=-I$(TOP_LEVEL)/lib/include -I$(MYINCLUDE) -I$(MYINCLUDE)/selector -I$(MYINCLUDE)/adapter -I$(MYINCLUDE)/core 

SOURCES=$(TOP_LEVEL)/lib/src/llvm/selector/cashesSelector.cpp \
				$(TOP_LEVEL)/lib/src/llvm/adapter/cygProfileAdapter.cpp 
#				$(TOP_LEVEL)/lib/src/llvm/Register.cpp

OBJECTS=$(SOURCES:.cpp=.o)

cashes-pass: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(PLUGIN_LDFLAGS) $(OBJECTS) $(CLANG_LIBS) $(LLVM_LDFLAGS_NOLIBS) -o cashesPass.so

.cpp.o:
	$(CXX) $(INSTRO_INCLUDES) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(PLUGIN_LDFLAGS) -c $< $(CLANG_LIBS) $(LLVM_LDFLAGS_NOLIBS) -o $@

cashes-pass-depr:
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(PLUGIN_LDFLAGS) cashesSelectorPass.cpp $(CLANG_LIBS) $(LLVM_LDFLAGS_NOLIBS) -o b_cashes.so
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(PLUGIN_LDFLAGS) cashesSelector.cpp $(CLANG_LIBS) $(LLVM_LDFLAGS_NOLIBS) -o b_cashesSel.so
	$(CXX) $(CXXFLAGS) $(LLVM_CXXFLAGS) $(PLUGIN_LDFLAGS) cygProfileAdapter.cpp $(CLANG_LIBS) $(LLVM_LDFLAGS_NOLIBS) -o b_cygProfileAdapter.so


clean:
	rm -rf *.so
	rm -rf *.o
