#ifndef INSTRO_CLANG_PASSFACTORY_H
#define INSTRO_CLANG_PASSFACTORY_H

#include <set>

#include "clang/Tooling/Core/Replacement.h"

#include "instro/PassFactory.h"
#include "instro/core/PassManager.h"

namespace clang {
// forward declaration
class ASTContext;
}

namespace InstRO {
namespace Clang {

// forward declaration
class ASTContextProvider;

/**
* The PassFactory exposes a handle to the created pass by a raw pointer.
* This does _not_ transfer ownership.
*/
class ClangPassFactory : public InstRO::PassFactory {
 public:
	ClangPassFactory(InstRO::PassManagement::PassManager* manager, clang::tooling::Replacements& reps)
			: InstRO::PassFactory(manager), replacements(reps){};

	InstRO::Pass* createDefaultInstrumentationAdapter(InstRO::Pass* input) { return nullptr; }

	Pass* createClangBlackAndWhiteListSelector(std::vector<std::string> blacklist, std::vector<std::string> whitelist);
	Pass* createClangCygProfileAdapter(InstRO::Pass* input);
	Pass* createClangMangledNameOutputAdapter(InstRO::Pass* input);

	/* We need this in order to lazily initialize the AST Context within the passes */
	void finishConstruction(clang::ASTContext* context);

 protected:
	std::set<InstRO::Clang::ASTContextProvider*> lazyContextProvidingMap;

 private:
	clang::tooling::Replacements& replacements;
};
}	// Clang
}	// INstRO
#endif
