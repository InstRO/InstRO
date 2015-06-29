#ifndef INSTRO_ROSE_PASSFACTORY_H
#define INSTRO_ROSE_PASSFACTORY_H

#include "instro/core/PassFactory.h"
#include "instro/core/PassManager.h"

#include "instro/example/ExamplePass.h"
#include "instro/example/CallPathSelector.h"
// #include "instro/test/selectors/BlackAndWhiteListSelector.h"

namespace InstRO
{
	namespace Test{

		class ExamplePassFactory :public PassFactory
{
	public:	
		ExamplePassFactory(PassManagement::PassManager * refManager) :PassFactory(refManager){};

		InstRO::Pass* createStringBasedSelector(std::vector<std::string> matchList) override { return NULL; };
		InstRO::Pass* createBooleanOrSelector(InstRO::Pass* inputA, InstRO::Pass* inputB) override { return NULL; };

		InstRO::Pass* createCallPathSelector(InstRO::Pass * from, InstRO::Pass * to)
		{
			return new InstRO::Pass(new Selectors::CallPathSelector(from, to));
		}

		// Convenience 
		InstRO::Pass* createProgramEntrySelector() override { return NULL; };
		InstRO::Pass* createFunctionSelector() override { return NULL; };
		InstRO::Pass* createGPIAdapter(InstRO::Pass* input) override { return NULL; };
};

	}
}
#endif