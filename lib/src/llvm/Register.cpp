
#include <iostream>

#include "instro/llvm/selector/CashesSelector.h"
#include "instro/llvm/adapter/CygProfileAdapter.h"
#include "instro/llvm/adapter/CygProfileAdapterT.h"
#include "instro/llvm/selector/ClangFrontendSelector.h"

#include "llvm/PassInfo.h"

template <typename pass, typename... Args>
llvm::Pass *create(Args... ctorArgs) {
	return new pass(ctorArgs...);
}

template <typename P, template <typename> class D>
llvm::Pass *create() {
	return new D<P>();
}

/*
 * Instantiation helper used to register passes with dependencies in clang
 *
 * TODO have the Dependence register itself
 */
template <template <typename> class Pass, typename Dependence>
struct InstantiationHelper {
	typedef Pass<Dependence> InstanceT;
	static llvm::Pass *create() { return new Pass<Dependence>(); }
};

llvm::Pass *createCygProfAndCashesSel() {
	return create<InstRO::LLVM::ClangFrontendSelector, InstRO::LLVM::CygProfileAdapterT>();
}

template <typename T>
struct InstRORegisterPass : public llvm::PassInfo {
	InstRORegisterPass(const char *passArg, const char *name, bool CFGOnly = false, bool isAnalysis = false)
			: llvm::PassInfo(name, passArg, &T::InstanceT::ID, llvm::PassInfo::NormalCtor_t(T::create), CFGOnly, isAnalysis) {
		llvm::PassRegistry::getPassRegistry()->registerPass(*this);
		std::cout << "In InstRORegisterPass constructor" << std::endl;
	}
};

/*
 * This is to register the passes inside the modular optimizer
 */
// InstRORegisterPass<InstRO::LLVM::CygProfileAdapterT<InstRO::LLVM::CashesSelector> > X("foobar",	"CygProfileAdapter
// World Pass");

llvm::RegisterPass<InstRO::LLVM::CashesSelector> Y("cash", "instro cash");

llvm::RegisterPass<InstRO::LLVM::ClangFrontendSelector> Z("asd", "sssks");

InstRORegisterPass<InstantiationHelper<InstRO::LLVM::CygProfileAdapterT, InstRO::LLVM::ClangFrontendSelector> > Test(
		"asdf", "asdasfwe");

