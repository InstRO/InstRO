#ifndef INSTRO_LLVM_INTERFACE_H
#define INSTRO_LLVM_INTERFACE_H

/* General top level interface definitions */
#include "instro/interface.h"

/* Compiler specifics */
#include "llvm/Pass.h"
#include "llvm/PassFactory.h"


/*
 * Within the LLVM namespace lives the implementation of the Clang and LLVM
 * dependent
 * components and infrastructure.
 */
namespace InstRO {
	namespace LLVM {
		/** InstRO for LLVM */
		class InstRO : public ::InstRO::InstRO {
		 public:
			PassFactory *getFactory();
		};
	}
}
#endif
