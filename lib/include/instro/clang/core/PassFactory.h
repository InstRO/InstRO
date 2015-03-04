#ifndef INSTRO_CLANG_PASSFACTORY_H
#define INSTRO_CLANG_PASSFACTORY_H

#include "instro/core/PassFactory.h"

#include "instro/clang/core/Pass.h"

namespace InstRO {
namespace Clang {
class PassFactory : public ::InstRO::Core::PassFactory {
 public:
 	PassFactory(::InstRO::Core::PassManagement::PassManager *manager) : ::InstRO::Core::PassFactory(manager){};
	Pass* createBlackAndWhiteListSelector(
			std::vector<std::string> rules) {};
	Pass* createBooleanOrSelector(::InstRO::Pass* inputA, ::InstRO::Pass* inputB) {};
	Pass* createProgramEntrySelector() {};
	Pass* createCygProfileAdapter(::InstRO::Pass* input) {};
};
}	// Clang
}	// INstRO
#endif
