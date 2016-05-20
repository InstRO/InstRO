/** \mainpage
*
 */
#include "instro/config.h"

#include "instro/Pass.h"
#include "instro/PassFactory.h"
#include "instro/Instrumentor.h"

#if INSTRO_USE_ROSE
#include "instro/rose/core/RosePassImplementation.h"
#include "instro/rose/RosePassFactory.h"
#include "instro/rose/RoseInstrumentor.h"
#endif

#if INSTRO_USE_CLANG
#include "instro/clang/ClangPassFactory.h"
#include "instro/clang/ClangInstrumentor.h"
#endif

