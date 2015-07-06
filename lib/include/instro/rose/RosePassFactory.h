#ifndef INSTRO_ROSE_PASSFACTORY_H
#define INSTRO_ROSE_PASSFACTORY_H

#include "instro/core/PassFactory.h"
#include "instro/core/PassManager.h"

#include "instro/rose/core/RosePassImplementation.h"
// #include "instro/rose/selectors/BlackAndWhiteListSelector.h"
#include "instro/rose/pass/selector/NameBasedSelector.h"
#include "instro/rose/pass/selector/CompoundSelector.h"
// #include "instro/rose/pass/adapter/GenericAdapter.h"
// #include "instro/rose/pass/adapter/CygProfileAdapter.h"
#include "instro/rose/pass/adapter/RoseConstructPrinter.h"

#include "rose.h"

namespace InstRO {
namespace Rose {

class RosePassFactory : public InstRO::PassFactory {
 protected:
	 RosePassImplementation* getPass(Pass* pass) {
		if (pass == NULL)
			return NULL;
		RosePassImplementation* rosePass = dynamic_cast<RosePassImplementation*>(pass->getPassImplementation());
		if (pass->getPassImplementation() != NULL && rosePass == NULL)
			throw std::string("Oh my god, what is going on");
		return rosePass;
	}
	SgProject* project;

 public:
	class GenericAdapterConfiguration {
	 public:
		GenericAdapterConfiguration() { loopPass = functionPass = loopBodyPass = NULL; }
		void instrumentFunctions(Pass* functionSelector) { functionPass = functionSelector; };
		void instrumentLoopConstruct(Pass* loopConstructSelector) { loopPass = loopConstructSelector; };
		void instrumentLoopBody(Pass* loopBodySelector) { loopBodyPass = loopBodySelector; };
		Pass* getFunctionSelector() { return functionPass; }
		Pass* getLoopConstructSelector() { return loopPass; }
		Pass* getLoopBodySelector() { return loopBodyPass; }

	 protected:
		Pass* loopPass, *functionPass, *loopBodyPass;
	};

 public:
	RosePassFactory(PassManagement::PassManager* refManager, SgProject* proj) : PassFactory(refManager), project(proj){};

	Pass * createConstructPrinter(InstRO::Pass *pass){
		Pass * newPass = new Pass(InstRO::Rose::Adapter::RoseConstructPrinter(pass));
		newPass->setPassName("InstRO::Rose::Adapter::RoseConstructPrinter");
		passManager->registerPass(newPass);
		return newPass;
	}
	
	Pass* createBlackAndWhiteListSelector(std::vector<std::string> rules) {
		std::vector<std::string> wlrules;
		std::vector<std::string> blrules;
		// get a matcher pass for the white list
		Pass* wlPass = new Pass(new Selector::NameBasedSelector(wlrules));
		wlPass->setPassName("InstRO::Rose::BlackAndWhiteList-WhiteList");
		passManager->registerPass(wlPass);

		Pass* blPass = new Pass(new Selector::NameBasedSelector(blrules));
		blPass->setPassName("InstRO::Rose::BlackAndWhiteList-BlackList");
		passManager->registerPass(blPass);

		Pass * compountPass = new Pass(new Selector::CompoundSelector(wlPass, blPass, Selector::CompoundSelector::CO_Or));
		compountPass->setPassName("InstRO::Rose:BlackAndWhiteList-Compound");
		passManager->registerPass(compountPass);

		return compountPass;
	}

	Pass* createBlackNWhiteSelector(std::string string) {
		std::vector<std::string> filters;
		filters.push_back(string);
		return createBlackAndWhiteListSelector(filters);
	};

	Pass* createBooleanOrSelector(Pass* inputA, Pass* inputB) override {
		Pass* newPass = new InstRO::Pass(new Rose::Selector::CompoundSelector(inputA, inputB, Selector::CompoundSelector::CO_Or));
		newPass->setPassName("InstRO::Rose::BooleanOrSelector");
		passManager->registerPass(newPass);
		/*		Pass * compoundPass=new Pass(new Selectors::CompoundSelector(getPass(inputA),getPass(inputB)));
				compoundPass->setPassName("ROSE_BooleanOr");
				compoundPass->setRequiresInput();
				compoundPass->setProvidesOutput();
				compoundPass->setOutputLevel(Core::ConstructLevelMin);
				compoundPass->registerInputPass(inputA,Core::ConstructLevelMin);
				compoundPass->registerInputPass(inputB,Core::ConstructLevelMin);
				passManager->registerPass(compoundPass);
				return compoundPass;*/
		return newPass;
	};

	Pass* createProgramEntrySelector() override { return NULL; };

	Pass* createCygProfileAdapter(Pass* input) {
	/*	Pass* newPass = new Pass(new Adapter::CygProfileAdapter(input));
		newPass->setPassName("InstRO::Rose::CygProfileAdapter");
		passManager->registerPass(newPass);*/
		return nullptr;
	};

	Pass* createGenericAdapter(Pass* functionSelection, Pass* loopSelection, Pass* branchingSelection) {
		// RosePass * roseFunctionSelectionPass,* roseLoopSelectionPass,*roseBranchingSelectionPass;
		/**/
		Adapters::GenericAdapter* roseAdapter =
				new Adapters::GenericAdapter(functionSelection, loopSelection, branchingSelection);
		Pass* newPass = new Pass(roseAdapter);
		newPass->setPassName("InstRO::Rose::GenericAdapter");
		passManager->registerPass(newPass);*/
		return nullptr;
	};

	Pass* createGenericAdapter(GenericAdapterConfiguration gac) {
		return createGenericAdapter(gac.getFunctionSelector(), gac.getLoopConstructSelector(), gac.getLoopBodySelector());
	};

	InstRO::Pass* createNameBasedSelector(std::vector<std::string> matchList) override {
		Pass* newPass = new Pass(new Selector::NameBasedSelector(matchList));
		newPass->setPassName("InstRO::Rose::NameBasedSelector");
		passManager->registerPass(newPass);
		return newPass;
	};

	InstRO::Pass* createNameBasedFilter(std::vector<std::string> matchList, Pass* filterInput) {
		Pass* newPass = new Pass(new Selector::NameBasedSelector(matchList, filterInput));
		newPass->setPassName("InstRO::Rose::NameBasedFilter");
		passManager->registerPass(newPass);
		return newPass;
	};
	/*
	InstRO::Pass* createBooleanOrSelector(InstRO::Pass* inputA,InstRO::Pass* inputB) override {
		return NULL;
	}*/

	// Convenience
	/*
	InstRO::Pass* createProgramEntrySelector() override {
		return NULL;
	}*/

	InstRO::Pass* createFunctionSelector() override { return NULL; }

	InstRO::Pass* createGPIAdapter(InstRO::Pass* input) override { return NULL; }
};
}
}
#endif