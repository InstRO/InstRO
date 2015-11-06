#ifndef INSTRO_ROSE_PASSFACTORY_H
#define INSTRO_ROSE_PASSFACTORY_H

#include "instro/core/PassManager.h"
#include "instro/core/PassFactory.h"

#include "instro/example/ExamplePass.h"
#include "instro/pass/selector/CallPathSelector.h"
#include "instro/example/CompoundSelector.h"
#include "instro/example/NameBasedSelector.h"
#include "instro/example/ExampleConstructPrinter.h"

namespace InstRO {
namespace Example {

class ExamplePassFactory : public InstRO::PassFactory {
 public:
	ExamplePassFactory(PassManagement::PassManager* refManager) : PassFactory(refManager){};

	InstRO::Pass* createConstructPrinter(InstRO::Pass* input) {
		InstRO::Pass* newPass = new Pass(new ExampleConstructPrinter(input));
		newPass->setPassName("InstRO::Example::ExampleConstructPrinter");
		passManager->registerPass(newPass);
		return newPass;
	};
	InstRO::Pass* createIdentifierMatcherSelector(std::vector<std::string> matchList) {
		InstRO::Pass* newPass = new InstRO::Pass(new Selector::NameBasedSelector(matchList));
		passManager->registerPass(newPass);
		newPass->setPassName("InstRO::Example::NameBasedSelector");
		return newPass;
	};

	InstRO::Pass* createCallPathSelector(InstRO::Pass* from, InstRO::Pass* to) {
		InstRO::Pass* newPass = new InstRO::Pass(new InstRO::Selector::CallPathSelector(from, to));
		newPass->setPassName("InstRO::Example::CallPathSelector");
		passManager->registerPass(newPass);
		return newPass;
	}

	virtual Pass* createBlackNWhiteSelector(std::string string) {return nullptr;};
	virtual Pass* createCygProfileAdapter(Pass* input) {return nullptr;};

	// Convenience
	InstRO::Pass* createFunctionSelector() { return NULL; };
	InstRO::Pass* createGPIAdapter(InstRO::Pass* input) { return NULL; };


};
}
}
#endif
