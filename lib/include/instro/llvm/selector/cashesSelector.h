#ifndef INSTRO_LLVM_CASHESSELECTOR_H
#define INSTRO_LLVM_CASHESSELECTOR_H

/* LLVM related includes */
#include "llvm/Analysis/CallGraphSCCPass.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/PassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

/* InstRO related includes */
#include "llvm/Pass.h"

namespace InstRO {
	namespace LLVM {

		/*
		 * Implements the CG analysis the InstRO Shadow Stack paper was based on
		 * Since we need the CallGraph of the module we need to have this is as a
		 * ModulePass!
		 */
		class CashesSelector : public ::InstRO::LLVM::Pass,
													 public llvm::ModulePass {
		 public:
			static char ID;

			CashesSelector();

			const char *getPassName() const { return pn.c_str(); }

			void getAnalysisUsage(llvm::AnalysisUsage &info);

			/*
			 * The runOnModule method implements the Cashes Selector behavior.
			 * It iterates over all functions and marks all functions which are a
			 * parent
			 * of a call
			 * graph node which has two or more predecessors to be instrumented.
			 */
			bool runOnModule(llvm::Module &m);

		 private:
			const std::string pn;

			std::vector<llvm::Function *> selectionSet;
		};

	}	// LLVM
}	// InstRO

#endif
