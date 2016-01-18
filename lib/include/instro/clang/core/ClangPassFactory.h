#ifndef INSTRO_CLANG_PASSFACTORY_H
#define INSTRO_CLANG_PASSFACTORY_H

#include <set>
#include "instro/core/PassFactory.h"
#include "instro/core/PassManager.h"
#include "instro/clang/core/Pass.h"
#include "instro/clang/core/ClangPassExecuter.h"

#include "instro/clang/selector/FunctionDefinitionSelector.h"
#include "instro/clang/selector/BlackWhitelistSelector.h"

#include "instro/clang/adapter/CygProfileAdapter.h"
#include "instro/clang/adapter/LLVMInputAdapter.h"

namespace InstRO {
namespace Clang {

/*
* The PassFactory exposes a handle to the created pass by a raw pointer.
* This does _not_ transfer ownership.
**/
class ClangPassFactory : public InstRO::PassFactory {
 public:
	ClangPassFactory(InstRO::PassManagement::PassManager* manager, clang::tooling::Replacements& reps)
			: InstRO::PassFactory(manager), replacements(reps){};

	InstRO::Pass* createDefaultInstrumentationAdapter(InstRO::Pass* input) { return nullptr; }

	Pass* createBlackAndWhiteListSelector(std::vector<std::string> blacklist, std::vector<std::string> whitelist);
	Pass* createFunctionDefinitionSelector();
	Pass* createCygProfileAdapter(InstRO::Pass* input);
	Pass* createLLVMInputAdapter(InstRO::Pass* input);

	/* We need this in order to lazily initialize the AST Context within the passes */
	void finishConstruction(clang::ASTContext* context) {
		for (auto p : lazyContextProvidingMap) {
			p->setASTContext(context);
		}
	}
 protected:
	std::set<InstRO::Clang::ASTContextProvider*> lazyContextProvidingMap;

 private:
	clang::tooling::Replacements& replacements;

};
}	// Clang
}	// INstRO
#endif
