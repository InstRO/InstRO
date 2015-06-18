/** \mainpage
 * InstRO is an instrumentation framework initially based on the ROSE compiler
 *infrastructure.
 * It is in development at TU Darmstadt, Germany.
 * Development started at RWTH Aachen, Germany.
 *
 * define INSTRUMENT_COMPILER_INSTANTIATED_TEMPLATES compiler flag to use InstRO
 *experimental
 * instrumentation of templated c++ code.
 * This depends on ROSE and its capability of handling template sources.
 *
 * Its goal is to provide a powerful tool for performance analysis of C/C++ and
 *Fortran codes.
 *
 *
 */
#ifndef USING_TEST
#ifndef USING_ROSE
#ifndef USING_LLVM
#define USING_ROSE
#endif
#else

#endif
#endif
// InstRO includes
// #ifndef DEFAULT_INSTRO
// #define USING_ROSE
// #define USING_LLVM
// #endif

#include "instro/core/Pass.h"
#include "instro/core/PassFactory.h"
#include "instro/core/Instrumentor.h"


//#include "instro/core/Pass.h"
//#include "instro/core/PassFactory.h"
//#include "instro/core/ConstructSet.h"
//#include "instro/core/Factory.h"
//#include "instro/core/Instrumentor.h"

//#include "instro/core/PassManager.h"


#ifdef USING_ROSE
#include "instro/rose/RosePass.h"
#include "instro/rose/RosePassFactory.h"
#include "instro/rose/RoseInstrumentor.h"
#include "instro/rose/adapters/CygProfileAdapter.h"
#include "instro/rose/selectors/CompoundSelectors.h"
#endif

#ifdef USING_LLVM
#include "instro/llvmInstRO.h"
#endif


#ifdef USING_TEST
#include "instro/test/TestInstrumentor.h"
#include "instro/test/TestPass.h"
#include "instro/test/TestPassFactory.h"
#endif