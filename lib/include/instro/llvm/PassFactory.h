#ifndef INSTRO_LLVM_PASSFACTORY_H
#define INSTRO_LLVM_PASSFACTORY_H

#include "llvm/Pass.h"

namespace InstRO {
	namespace LLVM {

		/** An LLVM Passes factory */
		class PassFactory : public ::InstRO::PassFactory {
		 public:
			Pass *createCygProfileAdapterPass(Pass *inputPass);
		};
	}
}

#endif
