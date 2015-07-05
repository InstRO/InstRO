#ifndef INSTRO_LLVM_CYGPROFILEADAPTERT_H
#define INSTRO_LLVM_CYGPROFILEADAPTERT_H

#include <iostream>

/* LLVM related includes */
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"

/* InstRO related includes */
#include "instro/llvm/core/ConstructSetPassing.h"

namespace InstRO {
namespace LLVM {
/*
 * Implements the cyg profile function adapter.
 * This version relies on the cashes selector.
 */
template <typename T>
class CygProfileAdapterT : public InstRO::LLVM::Core::ConstructSetPassing, public llvm::FunctionPass {
 public:
	CygProfileAdapterT();
	CygProfileAdapterT(T *inputSel);

	const char *getPassName() const { return pn.c_str(); }

	bool doInitialization(llvm::Module &m) override;
	bool runOnFunction(llvm::Function &f) override;

	bool doFinalization(llvm::Module &m) {}

	void getAnalysisUsage(llvm::AnalysisUsage &info) const;
	static char ID;

 private:
	typedef T TemplT;
	const std::string pn;
	const std::string exitName;
	const std::string entryName;

	T *inputSelector;

	llvm::CallInst *entryFunc;
	llvm::CallInst *exitFunc;
	llvm::Module *mod;
	llvm::CallInst *buildEntryCall(llvm::Function &f);
	llvm::CallInst *buildExitCall(llvm::Function &f, llvm::ReturnInst *ri);
	// XXX Why does that need to be a llvm::Twine&& ?
	llvm::CallInst *buildTCall(llvm::Function &f, llvm::Twine &&name, llvm::Instruction *insertBefore);

	llvm::Function *buildFunction(std::string fName);
};

}	// LLVM
}	// InstRO

#include "../src/llvm/adapter/CygProfileAdapterT.cpp"

#endif
