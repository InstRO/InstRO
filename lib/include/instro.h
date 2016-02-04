/** \mainpage
*
 */
#include "instro/config.h"

#include "instro/core/Pass.h"
#include "instro/core/PassFactory.h"
#include "instro/core/Instrumentor.h"

#if INSTRO_USE_MOCKUP
#include "instro/mockup/MockupInstrumentor.h"
//#include "instro/dummy/DummyPassFactory.h"
#endif

#if INSTRO_USE_ROSE
#include "instro/rose/core/RosePassImplementation.h"
#include "instro/rose/RosePassFactory.h"
#include "instro/rose/RoseInstrumentor.h"
#endif

#if INSTRO_USE_CLANG
#include "instro/clang/ClangPassFactory.h"
#include "instro/clang/ClangInstrumentor.h"
#endif

#if INSTRO_USE_LLVM
#include "instro/llvmInstRO.h"
#endif

