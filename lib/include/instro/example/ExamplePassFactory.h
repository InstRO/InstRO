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

	virtual Pass* createBlackNWhiteSelector(std::string string) {return nullptr;};
	virtual Pass* createCygProfileAdapter(Pass* input) {return nullptr;};

	// Convenience
	InstRO::Pass* createFunctionSelector() { return NULL; };
	InstRO::Pass* createGPIAdapter(InstRO::Pass* input) { return NULL; };


};
}
}
#endif
