#ifndef INSTRO_CLANG_PASSFACTORY_H
#define INSTRO_CLANG_PASSFACTORY_H

#include "instro/core/PassFactory.h"
#include "instro/core/PassManager.h"
#include "instro/clang/core/Pass.h"
#include "instro/clang/core/ClangPassExecuter.h"

/*
 * I guess we would need to know all the passes...
 */
#include "instro/clang/selector/FunctionDefinitionSelector.h"
#include "instro/clang/selector/BlackWhitelistSelector.h"

#include "instro/clang/adapter/CygProfileAdapter.h"

namespace InstRO {
namespace Clang {
/*
 * XXX Should a PassFactory return std::unique_ptr<Pass> instead of the raw
 * pointer
 * to a Pass? We know, that the Pass, if wrapped within a unique ptr will not
 * outlive
 * the overall InstRO Framework. So it should be safe to return a unique ptr,
 * but still
 * reference the Passes internally by raw pointers
 */
class PassFactory : public InstRO::Core::PassFactory {
 public:
	PassFactory(InstRO::Core::PassManagement::PassManager* manager,
							clang::tooling::Replacements& reps, InstRO::Clang::Core::PassManagement::ClangPassExecuter *executer)
			: InstRO::Core::PassFactory(manager), replacements(reps), executer(executer){};
	Pass* createBlackAndWhiteListSelector(std::vector<std::string> blacklist,
																				std::vector<std::string> whitelist);
	Pass* createBooleanOrSelector(InstRO::Pass* inputA, InstRO::Pass* inputB);
	Pass* createFunctionDefinitionSelector();
	Pass* createProgramEntrySelector(){};
	Pass* createCygProfileAdapter(InstRO::Pass* input);

 private:
	clang::tooling::Replacements& replacements;
	InstRO::Clang::Core::PassManagement::ClangPassExecuter *executer;
};
}	// Clang
}	// INstRO
#endif
