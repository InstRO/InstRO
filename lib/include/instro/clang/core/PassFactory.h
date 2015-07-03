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
#include "instro/clang/selector/BooleanCompoundSelector.h"

#include "instro/clang/adapter/CygProfileAdapter.h"
#include "instro/clang/adapter/LLVMInputAdapter.h"

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
class PassFactory : public InstRO::PassFactory {
 public:
	PassFactory(InstRO::PassManagement::PassManager* manager, clang::tooling::Replacements& reps,
							InstRO::Clang::PassManagement::VisitingClangPassExecuter* vExecuter,
							InstRO::Clang::PassManagement::NonVisitingClangPassExecuter* nvExecuter)
			: InstRO::PassFactory(manager), replacements(reps), vExecuter(vExecuter), nvExecuter(nvExecuter){};

	Pass* createBlackAndWhiteListSelector(std::vector<std::string> blacklist, std::vector<std::string> whitelist);
	Pass* createBooleanOrSelector(InstRO::Pass* inputA, InstRO::Pass* inputB);
	Pass* createFunctionDefinitionSelector();
	Pass* createProgramEntrySelector(){};
	Pass* createCygProfileAdapter(InstRO::Pass* input);
	Pass* createLLVMInputAdapter(InstRO::Pass* input);

	/**
	 * Introduced in the base class. So I had to provide some implementation..
	 */
	virtual InstRO::Pass* createFunctionSelector() override { return nullptr; };
	virtual InstRO::Pass* createGPIAdapter(InstRO::Pass* input) override { return nullptr; };
	virtual InstRO::Pass* createNameBasedSelector(std::vector<std::string> matchList) override { return nullptr; };

 private:
	clang::tooling::Replacements& replacements;
	InstRO::Clang::PassManagement::VisitingClangPassExecuter* vExecuter;
	InstRO::Clang::PassManagement::NonVisitingClangPassExecuter* nvExecuter;
};
}	// Clang
}	// INstRO
#endif
