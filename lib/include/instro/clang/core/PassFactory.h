#ifndef INSTRO_CLANG_PASSFACTORY_H
#define INSTRO_CLANG_PASSFACTORY_H

#include "instro/core/PassFactory.h"

#include "instro/clang/core/Pass.h"

/*
 * I guess we would need to know all the passes...
 */
#include "instro/clang/selector/FunctionDefinitionSelector.h"
//#include "instro/clang/selector/GrammarTraitSelector.h"

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
class PassFactory : public ::InstRO::Core::PassFactory {
 public:
	PassFactory(::InstRO::Core::PassManagement::PassManager* manager)
			: ::InstRO::Core::PassFactory(manager){};
	Pass* createBlackAndWhiteListSelector(std::vector<std::string> rules);
	Pass* createBooleanOrSelector(InstRO::Pass* inputA, InstRO::Pass* inputB);
	Pass* createFunctionDefinitionSelector();
	Pass* createProgramEntrySelector(){};
	Pass* createCygProfileAdapter(InstRO::Pass* input);
};
}	// Clang
}	// INstRO
#endif
