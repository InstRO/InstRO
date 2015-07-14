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
#ifndef USING_EXAMPLE
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

namespace InstRO {
namespace Core {};		 // Here we define the core interfaces and classes of instro
namespace Tooling {};	// Here we define the interface for the classes of the tooling layer. Implementations may reside
											 // within a tools namespace
namespace Extra {};		 // This namespace is used for extensions associated with InstRO, such as the visual pass manager
namespace Utility {};	// Contains util functions, such as reg-expression, file readers, etc.
namespace Rose {};		 // Contains the implementation for ROSE
namespace LLVM {};		 // Contains the implementation for LLVM
namespace CLANG {};		 // Contains the implementation for CLANG
namespace Example {};	// An example implementation and usage of the InstRO interface without actual functionality. This
											 // serves to demonstrate what must be implemented, to obtain a minimal functional InstRo
}

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
#include "instro/rose/core/RosePassImplementation.h"
#include "instro/rose/RosePassFactory.h"
#include "instro/rose/RoseInstrumentor.h"
#include "instro/rose/pass/adapter/RoseConstructPrinter.h"
#endif

#ifdef USING_LLVM
#include "instro/llvmInstRO.h"
#endif

#ifdef USING_EXAMPLE
//#include "instro/example/ExampleConstructProvider.h"
#include "instro/example/ExampleInstrumentor.h"
#include "instro/example/ExamplePass.h"
#include "instro/example/ExamplePassFactory.h"
#endif
