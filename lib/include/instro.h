/** \mainpage
 *
 */
#include "instro/config.h"

#include "instro/Instrumentor.h"
#include "instro/Pass.h"
#include "instro/PassFactory.h"

#if INSTRO_USE_ROSE
#include "instro/rose/RoseInstrumentor.h"
#include "instro/rose/RosePassFactory.h"
#include "instro/rose/core/RosePassImplementation.h"
#endif

#if INSTRO_USE_CLANG
#include "instro/clang/ClangInstrumentor.h"
#include "instro/clang/ClangPassFactory.h"
#endif
