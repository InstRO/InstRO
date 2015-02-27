#include "cashesSelector.h"

char ::InstRO::LLVM::CashesSelector::ID = 0;

::InstRO::LLVM::CashesSelector::CashesSelector() : ModulePass(ID){}

void ::InstRO::LLVM::CashesSelector::getAnalysisUsage(llvm::AnalysisUsage &info) {
	/*
	 * For the use within Clang I tried to use the CallGraphWrapperPass as a
	 * required
	 * pass, but it crashed in Clang. It generally behaved weirdly, or at least
	 * unexpected
	 * since it complained all the time, that this pass was depending on
	 * something, which
	 * was not mentioned as required. Which simply was a lie.
	 * Since we are not changing anything, we are not invalidating any analysis.
	 */
	info.setPreservesAll();
}

/*
 * The runOnModule method implements the Cashes Selector behavior.
 * It iterates over all functions and marks all functions which are a parent of
 * a call
 * graph node which has two or more predecessors to be instrumented.
 */
bool ::InstRO::LLVM::CashesSelector::runOnModule(llvm::Module &m) {
	llvm::CallGraph cg(m);
	for (auto &f : m.getFunctionList()) {
		const llvm::CallGraphNode *cgf = cg[&f];
		for (auto it = cgf->begin(); it != cgf->end(); ++it) {
			llvm::CallGraphNode *cgn = it->second;
			/*
			 * I have no clue why this -1 is necessary. However, first primitive
			 * experiments show, that it works.
			 */
			if (cgn->getNumReferences() - 1 > 1) {
				/*
				 * XXX What exactly does this mean?
				 */
				if (cgn->getFunction() == NULL) {
					continue;
				}
				selectionSet.push_back(&f);
				// if we found one reason to instrument we don't need a second.
				continue;
			}
		}
	}
	llvm::errs() << "CashesSelector marked " << selectionSet.size() << " nodes\n";
	return false;
}
