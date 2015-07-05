#ifndef INSTRO_ROSE_PASSFACTORY_H
#define INSTRO_ROSE_PASSFACTORY_H

#include "instro/core/PassFactory.h"
#include "instro/core/PassManager.h"

#include "instro/example/ExamplePass.h"
#include "instro/example/CallPathSelector.h"
#include "instro/example/NameBasedSelector.h"
#include "instro/example/ExampleConstructPrinter.h"
// #include "instro/test/selectors/BlackAndWhiteListSelector.h"

namespace InstRO {
namespace Example {

class ExamplePassFactory : public PassFactory {
 public:
	ExamplePassFactory(PassManagement::PassManager* refManager) : PassFactory(refManager){};

	InstRO::Pass* createConstructPrinter(InstRO::Pass* input) {
		InstRO::Pass* newPass = new Pass(new ExampleConstructPrinter(input));
		newPass->setPassName("InstRO::Example::ExampleConstructPrinter");
		passManager->registerPass(newPass);
		return newPass;
	};
	InstRO::Pass* createNameBasedSelector(std::vector<std::string> matchList) override {
		InstRO::Pass* newPass = new InstRO::Pass(new Selectors::NameBasedSelector(matchList));
		passManager->registerPass(newPass);
		newPass->setPassName("InstRO::Example::NameBasedSelector");
		return newPass;
	};
	InstRO::Pass* createBooleanOrSelector(InstRO::Pass* inputA, InstRO::Pass* inputB) override { return NULL; };

	InstRO::Pass* createCallPathSelector(InstRO::Pass* from, InstRO::Pass* to) {
		InstRO::Pass* newPass = new InstRO::Pass(new Selectors::CallPathSelector(from, to));
		newPass->setPassName("InstRO::Example::CallPathSelector");
		passManager->registerPass(newPass);
		return newPass;
	}

	// Convenience
	InstRO::Pass* createProgramEntrySelector() override { return NULL; };
	InstRO::Pass* createFunctionSelector() override { return NULL; };
	InstRO::Pass* createGPIAdapter(InstRO::Pass* input) override { return NULL; };
};
}
}
#endif