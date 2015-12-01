/** \mainpage
*
 */
#include "instro/config.h"

#include "instro/core/Pass.h"
#include "instro/core/PassFactory.h"
#include "instro/core/Instrumentor.h"

#ifdef USE_ROSE
#include "instro/rose/core/RosePassImplementation.h"
#include "instro/rose/RosePassFactory.h"
#include "instro/rose/RoseInstrumentor.h"
#include "instro/rose/pass/adapter/RoseConstructPrinter.h"
#endif

#ifdef USING_LLVM
#include "instro/llvmInstRO.h"
#endif

