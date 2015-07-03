#ifndef INSTRO_LLVM_FUNCTIONDEFINITIONSELECTOR
#define INSTRO_LLVM_FUNCTIONDEFINITIONSELECTOR

#include <iostream>

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "instro/llvm/core/ConstructSetPassing.h"

namespace InstRO {
namespace LLVM {
class FunctionDefinitionSelector : public InstRO::LLVM::Core::ConstructSetPassing, public llvm::FunctionPass {
 public:
	static char ID;

	FunctionDefinitionSelector();
	const char *getPassName() const { return pn.c_str(); }
	void getAnalysisUsage(llvm::AnalysisUsage &info);

	bool runOnFunction(llvm::Function &function) override;

 private:
	std::string pn;
};
}
}

#endif
