#ifndef INSTRO_ROSE_PASSFACTORY_H
#define INSTRO_ROSE_PASSFACTORY_H

#include "instro/core/PassFactory.h"
#include "instro/core/PassManager.h"

#include "instro/test/TestPass.h"
// #include "instro/test/selectors/BlackAndWhiteListSelector.h"

namespace InstRO
{
	namespace Test{

class TestPassFactory:public PassFactory
{
	public:	
		TestPassFactory(PassManagement::PassManager * refManager):PassFactory(refManager){};

		InstRO::Pass* createStringBasedSelector(std::vector<std::string> matchList) override { return NULL; };
		InstRO::Pass* createBooleanOrSelector(InstRO::Pass* inputA, InstRO::Pass* inputB) override { return NULL; };

		// Convenience 
		InstRO::Pass* createProgramEntrySelector() override { return NULL; };
		InstRO::Pass* createFunctionSelector() override { return NULL; };
		InstRO::Pass* createGPIAdapter(InstRO::Pass* input) override { return NULL; };
};

	}
}
#endif