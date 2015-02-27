
#include "instro/llvm/selector/cashesSelector.h"
#include "instro/llvm/adapter/cygProfileAdapter.h"

#include "llvm/PassInfo.h"

template <typename pass, typename... Args>
llvm::Pass *create(Args... ctorArgs) {
	return new pass(ctorArgs...);
}

template <typename T>
struct InstRORegisterPass : public llvm::PassInfo {
	InstRORegisterPass(const char *passArg, const char *name,
										 bool CFGOnly = false, bool isAnalysis = false)
			: llvm::PassInfo(name, passArg, &T::ID,
											 llvm::PassInfo::NormalCtor_t(
													 create<::InstRO::LLVM::CygProfileAdapter>(
															 new ::InstRO::LLVM::CashesSelector())),
											 CFGOnly, isAnalysis) {
		llvm::PassRegistry::getPassRegistry()->registerPass(*this);
	}
};

/*
 * This is to register the passes inside the modular optimizer
 */
InstRORegisterPass<::InstRO::LLVM::CygProfileAdapter> X(
		"foobar", "CygProfileAdapter World Pass");
InstRORegisterPass<::InstRO::LLVM::CashesSelector> Y("cash", "instro cash");

