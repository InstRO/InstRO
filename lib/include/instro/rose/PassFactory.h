#ifndef INSTRO_ROSE_PASSFACTORY_H
#define INSTRO_ROSE_PASSFACTORY_H

#include "instro/core/PassFactory.h"
#include "instro/core/PassManager.h"

#include "instro/rose/Pass.h"

namespace InstRO
{
	namespace Rose{

class RosePassFactory:public PassFactory
{
	public:	
		RosePassFactory(PassManagement::PassManager * refManager):PassFactory(refManager){};
		RosePass * createBlackNWhiteFilter(Pass * input)
		{
			return NULL;
			//	RosePass * pass=new RosePass(input);
			//		refToGobalPassManager->registerPass(pass);
					
			//					refToGobalPassManager->addDependency(pass,input,1);
			//,std::vector<Pass*>().push_back(input));
		}
		RosePass * createBlackNWhiteSelector(std::string string){return NULL;};
		RosePass * createBooleanOrSelector(Pass * inputA,Pass * inputB){return NULL;};
		RosePass * createProgramEntrySelector(){return NULL;};
		RosePass * createCygProfileAdapter(Pass * input){return NULL;};
};

	}
}
#endif