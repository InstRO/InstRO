
#include <iostream>

#include "instro/llvm/selector/FunctionDefinitionSelector.h"
#include "instro/llvm/selector/ClangFrontendSelector.h"
#include "instro/llvm/selector/CashesSelector.h"
#include "instro/llvm/adapter/CygProfileAdapter.h"
#include "instro/llvm/adapter/CygProfileAdapterT.h"

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

llvm::RegisterPass<InstRO::LLVM::FunctionDefinitionSelector> FDEFSelector("fdef-sel", "Function Definition Selector");

llvm::RegisterPass<InstRO::LLVM::CashesSelector> CGBSelector("cgb-sel", "Callgraph based selector");

llvm::RegisterPass<InstRO::LLVM::ClangFrontendSelector> CFESelector("cfe-sel", "CLang Frontend Selector");

InstRORegisterPass<InstantiationHelper<InstRO::LLVM::CygProfileAdapterT, InstRO::LLVM::CashesSelector> >
		CGBInstRO("cgb-instro", "Callgraph analysis based function instrumentation");

InstRORegisterPass<InstantiationHelper<InstRO::LLVM::CygProfileAdapterT, InstRO::LLVM::FunctionDefinitionSelector> >
		FDEFInstRO("fdef-instro", "Simple function definition instrumentation");

