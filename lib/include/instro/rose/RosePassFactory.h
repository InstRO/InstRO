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

class RosePassFactory:public Core::PassFactory
{
protected:
	RosePass * getPass(Pass * pass)
	{
		if (pass==NULL) return NULL;
		RosePass * rosePass=dynamic_cast<RosePass*>(pass->getPassImplementation());
		if (pass->getPassImplementation()!=NULL && rosePass==NULL) throw std::string("Oh my god, what is going on");
		return rosePass;
	}
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

		Pass * createBlackAndWhiteListSelector(std::vector<std::string> rules)
		{
			Pass * bwlPass = new Pass(new Selectors::BlackAndWhiteListSelector(rules));
			bwlPass->setPassName("ROSE_BlackAndWhiteList");
			bwlPass->setRequiresInput(false);
			bwlPass->setProvidesOutput();
			bwlPass->setOutputLevel(Core::ConstructLevelLiteral);
			passManager->registerPass(bwlPass);
			return bwlPass;
		}

		Pass * createBlackNWhiteSelector(std::string string)
		{
			std::vector<std::string> filters;
			filters.push_back(string);
			return createBlackAndWhiteListSelector(filters);
		};

		Pass * createBooleanOrSelector(Pass * inputA,Pass * inputB)
		{
			
			Pass * compoundPass=new Pass(new Selectors::CompoundSelector(getPass(inputA),getPass(inputB)));
			compoundPass->setPassName("ROSE_BooleanOr");
			compoundPass->setRequiresInput();
			compoundPass->setProvidesOutput();
			compoundPass->setOutputLevel(Core::ConstructLevelMin);
			compoundPass->registerInputPass(inputA,Core::ConstructLevelMin);
			compoundPass->registerInputPass(inputB,Core::ConstructLevelMin);
			passManager->registerPass(compoundPass);
			return compoundPass;

		};

		Pass * createProgramEntrySelector(){return NULL;};

		Pass * createCygProfileAdapter(Pass * input)
		{
			Pass * newPass=new Pass(new Adapters::CygProfileAdapter(getPass(input)));
			newPass->setRequiresInput();
			newPass->setProvidesOutput(false);
			newPass->setPassName("ROSE_CygProfileAdapter");
			newPass->registerInputPass(input,Core::ConstructLevelStatement);
			return newPass;
		};

		Pass * createGenericAdapter(Pass * functionSelection, Pass * loopSelection, Pass * branchingSelection)
		{
			//RosePass * roseFunctionSelectionPass,* roseLoopSelectionPass,*roseBranchingSelectionPass;
			
			Adapters::GenericAdapter * roseAdapter=new Adapters::GenericAdapter(getPass(functionSelection),getPass(loopSelection),getPass(branchingSelection));
			Pass * newPass=new Pass(roseAdapter);
			newPass->setRequiresInput();
			newPass->setProvidesOutput(false);
			newPass->setPassName("ROSE_GenericAdapter");
			if (functionSelection!=NULL) newPass->registerInputPass(functionSelection,Core::ConstructLevelStatement);
			if (loopSelection!=NULL) newPass->registerInputPass(loopSelection,Core::ConstructLevelStatement);
			if (branchingSelection!=NULL) newPass->registerInputPass(branchingSelection,Core::ConstructLevelStatement);
			
			passManager->registerPass(newPass);
			return newPass;
		};

		Pass * createGenericAdapter(GenericAdapterConfiguration gac)
		{
			return createGenericAdapter(gac.getFunctionSelector(),gac.getLoopConstructSelector(),gac.getLoopBodySelector());
		};

};

	}
}
#endif