#ifndef INSTRO_LLVM_PASSFACTORY_H
#define INSTRO_LLVM_PASSFACTORY_H

#include "llvm/Pass.h"

namespace InstRO {
namespace LLVM {

/** An LLVM Passes factory */
class PassFactory : public ::InstRO::PassFactory {
 public:
	InstRO::Pass* createDefaultInstrumentationAdapter(InstRO::Pass* input) {
		return nullptr;
	}
	Pass *createCygProfileAdapterPass(Pass *inputPass);
};
}
}

#endif
