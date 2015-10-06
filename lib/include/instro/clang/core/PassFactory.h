#ifndef INSTRO_CLANG_PASSFACTORY_H
#define INSTRO_CLANG_PASSFACTORY_H

#include <set>
#include "instro/core/PassFactory.h"
#include "instro/core/PassManager.h"
#include "instro/clang/core/Pass.h"
#include "instro/clang/core/ClangPassExecuter.h"

/*
 * I guess we would need to know all the passes...
 */
#include "instro/clang/selector/FunctionDefinitionSelector.h"
#include "instro/clang/selector/BlackWhitelistSelector.h"
#include "instro/clang/selector/BooleanCompoundSelector.h"

#include "instro/clang/adapter/CygProfileAdapter.h"
#include "instro/clang/adapter/LLVMInputAdapter.h"

namespace InstRO {
namespace Clang {
/*
 * The PassFactory exposes a handle to the created pass by a raw pointer.
 * This does _not_ transfer ownership.
 **/
class PassFactory : public InstRO::PassFactory {
 public:
	PassFactory(InstRO::PassManagement::PassManager* manager, clang::tooling::Replacements& reps)
			: InstRO::PassFactory(manager), replacements(reps){};

	Pass* createBlackAndWhiteListSelector(std::vector<std::string> blacklist, std::vector<std::string> whitelist);
	Pass* createBooleanOrSelector(InstRO::Pass* inputA, InstRO::Pass* inputB);
	Pass* createFunctionDefinitionSelector();
	Pass* createProgramEntrySelector(){};
	Pass* createCygProfileAdapter(InstRO::Pass* input);
	Pass* createLLVMInputAdapter(InstRO::Pass* input);

	/**
	 * Introduced in the base class. So I had to provide some implementation..
	 */
//	virtual InstRO::Pass* createFunctionSelector() override { return nullptr; };
//	virtual InstRO::Pass* createGPIAdapter(InstRO::Pass* input) override { return nullptr; };
//	virtual InstRO::Pass* createNameBasedSelector(std::vector<std::string> matchList) override { return nullptr; };

	/* We need this in order to lazily initialize the AST Context within the passes */
	void finishConstruction(clang::ASTContext *context){
		for(auto p : lazyContextProvidingMap){
			p->setASTContext(context);
		}
	}
		
 private:
	clang::tooling::Replacements& replacements;
	std::set<InstRO::Clang::ASTContextProvider *> lazyContextProvidingMap;
};
}	// Clang
}	// INstRO
#endif
