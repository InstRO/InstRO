#ifndef INSTRO_LLVM_CYGPROFILEADAPTER_H
#define INSTRO_LLVM_CYGPROFILEADAPTER_H

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"

namespace InstRO {
	namespace LLVM {
		/*
		 * Implements the cyg profile function adapter.
		 * This version relies on the cashes selector.
		 */
		class CygProfileAdapter : public ::InstRO::LLVM::Pass,
															public llvm::FunctionPass {
		 public:
			CygProfileAdapter(::InstRO::LLVM::Pass *inputSel);

			const char *getPassName() const { return pn.c_str(); }

			bool doInitialization(llvm::Module &m);
			bool runOnFunction(llvm::Function &f) override;

			bool doFinalization(llvm::Module &m) {}

			void getAnalysisUsage(llvm::AnalysisUsage &info) const {}

			static char ID;

		 private:
			const std::string pn;
			const std::string exitName;
			const std::string entryName;

			llvm::CallInst *entryFunc;
			llvm::CallInst *exitFunc;
			llvm::Module *mod;
			llvm::CallInst *buildEntryCall(llvm::Function &f);
			llvm::CallInst *buildExitCall(llvm::Function &f, llvm::ReturnInst *ri);
			// XXX Why does that need to be a llvm::Twine&& ?
			llvm::CallInst *buildTCall(llvm::Function &f, llvm::Twine &&name,
																 llvm::Instruction *insertBefore);

			llvm::Function *buildFunction(std::string fName);
		};

	}	// LLVM
}	// InstRO
#endif
