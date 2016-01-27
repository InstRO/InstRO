/** \mainpage
*
 */
#include "instro/config.h"

#include "instro/core/Pass.h"
#include "instro/core/PassFactory.h"
#include "instro/core/Instrumentor.h"

#if INSTRO_USE_DUMMYCOMPILER
#include "instro/dummy/DummyPassFactory.h"
#endif

#if INSTRO_USE_ROSE
#include "instro/rose/core/RosePassImplementation.h"
#include "instro/rose/RosePassFactory.h"
#include "instro/rose/RoseInstrumentor.h"
#endif

#if INSTRO_USE_CLANG
#include "instro/clang/core/ClangPassFactory.h"
#include "instro/clang/ClangInstrumentor.h"
#endif

#ifdef USING_LLVM
#include "instro/llvmInstRO.h"
#endif

