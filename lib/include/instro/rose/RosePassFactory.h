#ifndef INSTRO_ROSE_PASSFACTORY_H
#define INSTRO_ROSE_PASSFACTORY_H

#include "instro/core/PassFactory.h"
#include "instro/core/PassManager.h"

#include "instro/rose/RosePass.h"
#include "instro/rose/selectors/BlackAndWhiteListSelector.h"

namespace InstRO
{
	namespace Rose{

class RosePassFactory:public PassFactory
{
	public:	
		RosePassFactory(PassManagement::PassManager * refManager):PassFactory(refManager){};
		RosePass * createBlackAndWhiteListSelector(std::vector<std::string> rules)
		{
			Selectors::BlackAndWhiteListSelector * bnwlSelector=new Selectors::BlackAndWhiteListSelector(rules);
			refToGobalPassManager->registerPass(bnwlSelector);
			return bnwlSelector;
		}
		RosePass * createBlackNWhiteSelector(std::string string){return NULL;};
		RosePass * createBooleanOrSelector(Pass * inputA,Pass * inputB){return NULL;};
		RosePass * createProgramEntrySelector(){return NULL;};
		RosePass * createCygProfileAdapter(Pass * input){return NULL;};
};

	}
}
#endif