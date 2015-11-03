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

INSTRUMENTOR_SOURCE= $(top_srcdir)/examples/ClangSrcToSrcFunctionInstrumentor/Instrumentor.cpp

## Manually crafted Clang stuff!
# FIXME Figure out how this can be done using actually autotools
ClangInstRO:
	$(CXX) $(LLVM_CXXFLAGS_FLAGS) -I$(LLVM_SRC_INCLUDE_PATH) -I$(CLANG_SRC_INCLUDE_PATH) -I$(BOOST_INCLUDEDIR) $(AM_CPPFLAGS) $(INSTRUMENTOR_SOURCE) $(top_builddir)/lib/libInstRO_clang.a $(top_builddir)/lib/libInstRO_core.a $(CLANG_LIBS_LIST) $(LLVM_LDFLAGS) -o $@


