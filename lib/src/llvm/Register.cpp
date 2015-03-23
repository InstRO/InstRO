
#include <iostream>

#include "instro/llvm/selector/CashesSelector.h"
#include "instro/llvm/adapter/CygProfileAdapter.h"
#include "instro/llvm/adapter/CygProfileAdapterT.h"

#include "llvm/PassInfo.h"


template <typename pass, typename... Args>
llvm::Pass *create(Args... ctorArgs) {
	return new pass(ctorArgs...);
}

llvm::Pass *createCygProfAndCashesSel(){
return new InstRO::LLVM::CygProfileAdapterT<InstRO::LLVM::CashesSelector>();
//return create<InstRO::LLVM::CygProfileAdapter>(new InstRO::LLVM::CashesSelector());
}


template <typename T>
struct InstRORegisterPass : public llvm::PassInfo {
	InstRORegisterPass(const char *passArg, const char *name, bool CFGOnly = false, bool isAnalysis = false)
			: llvm::PassInfo(
						name, passArg, &T::ID,
						llvm::PassInfo::NormalCtor_t(createCygProfAndCashesSel),
						CFGOnly, isAnalysis) {
		llvm::PassRegistry::getPassRegistry()->registerPass(*this);
	std::cout << "In InstRORegisterPass constructor" << std::endl;
	}
};



/*
 * This is to register the passes inside the modular optimizer
 */
InstRORegisterPass<InstRO::LLVM::CygProfileAdapterT<InstRO::LLVM::CashesSelector> > X("foobar", "CygProfileAdapter World Pass");

llvm::RegisterPass<InstRO::LLVM::CashesSelector> Y("cash", "instro cash");

