#ifndef INSTRO_ROSE_PASSFACTORY_H
#define INSTRO_ROSE_PASSFACTORY_H

#include "instro/core/PassFactory.h"
#include "instro/core/PassManager.h"

#include "instro/rose/RosePass.h"
#include "instro/rose/selectors/BlackAndWhiteListSelector.h"
#include "instro/rose/selectors/CompoundSelectors.h"
#include "instro/rose/adapters/GenericAdapter.h"
#include "instro/rose/adapters/CygProfileAdapter.h"

namespace InstRO
{
	namespace Rose{

class RosePassFactory:public PassFactory
{
public:
	class GenericAdapterConfiguration {
	public:
		GenericAdapterConfiguration(){loopPass=functionPass=loopBodyPass=NULL;}
		void instrumentFunctions(Pass * functionSelector){functionPass=functionSelector;};
		void instrumentLoopConstruct(Pass * loopConstructSelector){loopPass=loopConstructSelector;};
		void instrumentLoopBody(Pass * loopBodySelector){loopBodyPass=loopBodySelector;};
		Pass * getFunctionSelector(){return functionPass;}
		Pass * getLoopConstructSelector(){return loopPass;}
		Pass * getLoopBodySelector(){return loopBodyPass;}
	protected:
		Pass * loopPass,* functionPass,*loopBodyPass;
	};
	public:	
		RosePassFactory(PassManagement::PassManager * refManager):PassFactory(refManager){};
		RosePass * createBlackAndWhiteListSelector(std::vector<std::string> rules)
		{
			Selectors::BlackAndWhiteListSelector * bnwlSelector=new Selectors::BlackAndWhiteListSelector(rules);
			refToGobalPassManager->registerPass(bnwlSelector);
			return bnwlSelector;
		}
		RosePass * createBlackNWhiteSelector(std::string string)
		{
			std::vector<std::string> filters;
			filters.push_back(string);
			return createBlackAndWhiteListSelector(filters);
		};
		RosePass * createBooleanOrSelector(Pass * inputA,Pass * inputB){
			Selectors::CompoundSelector * sel=new Selectors::CompoundSelector(dynamic_cast<RosePass*>(inputA),dynamic_cast<RosePass*>(inputB));
			refToGobalPassManager->registerPass(sel);
			return sel;

		};
		RosePass * createProgramEntrySelector(){return NULL;};
		RosePass * createCygProfileAdapter(Pass * input)
		{
//			::InstRO::Rose::Adapters::
			return NULL;
		};
		RosePass * createGenericAdapter(Pass * functionSelection, Pass * loopSelection, Pass * branchingSelection)
		{
			Adapters::GenericAdapter *newAdapter=new Adapters::GenericAdapter(dynamic_cast<RosePass*>(functionSelection),dynamic_cast<RosePass*>(loopSelection),dynamic_cast<RosePass*>(branchingSelection));
			refToGobalPassManager->registerPass(newAdapter);
			return newAdapter;
		};
		RosePass * createGenericAdapter(GenericAdapterConfiguration gac){return createGenericAdapter(gac.getFunctionSelector(),gac.getLoopConstructSelector(),gac.getLoopBodySelector());};

};

	}
}
#endif