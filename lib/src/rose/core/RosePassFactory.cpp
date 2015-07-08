#include "instro/core/Pass.h"
#include "instro/core/Singleton.h"
#include "instro/rose/RosePassFactory.h"

#include "rose.h"

namespace InstRO {
namespace Rose {

InstRO::Pass* RosePassFactory::createConstructLoweringElevator(InstRO::Pass* pass, InstRO::Core::ConstructLevelType level) {
	Pass* newPass = new Pass(new InstRO::Rose::Selector::ConstructLoweringElevator(pass, level));
	newPass->setPassName("InstRO::Rose::Selector::ConstructLoweringElevator");
	passManager->registerPass(newPass);
	return newPass;
}
Pass* RosePassFactory::createConstructRaisingElevator(InstRO::Pass* pass, InstRO::Core::ConstructLevelType level) {
	Pass* newPass = new Pass(new InstRO::Rose::Selector::ConstructRaisingElevator(pass, level));
	newPass->setPassName("InstRO::Rose::Selector::ConstructRaisingElevator");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* RosePassFactory::createConstructPrinter(InstRO::Pass* pass) {
	Pass* newPass = new Pass(new InstRO::Rose::Adapter::RoseConstructPrinter(pass));
	newPass->setPassName("InstRO::Rose::Adapter::RoseConstructPrinter");
	passManager->registerPass(newPass);
	return newPass;
}



Pass* RosePassFactory::createFunctionBlackAndWhiteListSelector(std::vector<std::string> rules) {
	std::vector<std::string> wlrules;
	std::vector<std::string> blrules;
	// get a matcher pass for the white list
	Pass* wlPass = new Pass(new Selector::NameBasedSelector(wlrules));
	wlPass->setPassName("InstRO::Rose::FunctionBlackAndWhiteListSelector-WhiteList");
	passManager->registerPass(wlPass);

	Pass* blPass = new Pass(new Selector::NameBasedSelector(blrules));
	blPass->setPassName("InstRO::Rose::FunctionBlackAndWhiteListSelector-BlackList");
	passManager->registerPass(blPass);

	Pass* compountPass = new Pass(new Selector::CompoundSelector(wlPass, blPass, Selector::CompoundSelector::CO_Or));
	compountPass->setPassName("InstRO::Rose:FunctionBlackAndWhiteListSelector-Compound");
	passManager->registerPass(compountPass);

	return compountPass;
}
/*
Pass* RosePassFactory::createBlackNWhiteSelector(std::string string) {
	std::vector<std::string> filters;
	filters.push_back(string);
	return createBlackAndWhiteListSelector(filters);
};*/

Pass* RosePassFactory::createBooleanOrSelector(Pass* inputA, Pass* inputB) {
	Pass* newPass =
		new InstRO::Pass(new Rose::Selector::CompoundSelector(inputA, inputB, Selector::CompoundSelector::CO_Or));
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

Pass* RosePassFactory::createProgramEntrySelector() { 
	return NULL; 
};


Pass* RosePassFactory::createGenericAdapter(Pass* functionSelection, Pass* loopSelection, Pass* branchingSelection) {
	// RosePass * roseFunctionSelectionPass,* roseLoopSelectionPass,*roseBranchingSelectionPass;
	/*
	Adapter::GenericAdapter* roseAdapter =
	new Adapters::GenericAdapter(functionSelection, loopSelection, branchingSelection);
	Pass* newPass = new Pass(roseAdapter);
	newPass->setPassName("InstRO::Rose::GenericAdapter");
	passManager->registerPass(newPass);*/
	return nullptr;
};

InstRO::Pass* RosePassFactory::createGenericAdapter(GenericAdapterConfiguration gac) {
	return createGenericAdapter(gac.getFunctionSelector(), gac.getLoopConstructSelector(), gac.getLoopBodySelector());
};

InstRO::Pass* RosePassFactory::createIdentifyerSelector(std::vector<std::string> matchList) {
	Pass* newPass = new Pass(new Selector::NameBasedSelector(matchList));
	newPass->setPassName("InstRO::Rose::IdentifyerSelector");
	passManager->registerPass(newPass);
	return newPass;
};


InstRO::Pass* RosePassFactory::createIdentifyerFilter(std::vector<std::string> matchList, InstRO::Pass* filterInput) {
	Pass* newPass = new Pass(new Selector::NameBasedSelector(matchList, filterInput));
	newPass->setPassName("InstRO::Rose::IdentifyerFilter");
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

InstRO::Pass* RosePassFactory::createFunctionSelector() { return NULL; }

InstRO::Pass* RosePassFactory::createGPIAdapter(InstRO::Pass* input) { return NULL; }
InstRO::Pass* RosePassFactory::createIdentifyerFilter(std::vector<std::string> matchList, Pass* filterInput){ return NULL; }
InstRO::Pass* RosePassFactory::createStringSelector(std::vector<std::string> matchList){ return NULL; }
InstRO::Pass* RosePassFactory::createFunctionBlackAndWhiteListFilter(std::vector<std::string> matchList, InstRO::Pass* inputB) { return NULL; }
InstRO::Pass* RosePassFactory::createBooleanAndSelector(InstRO::Pass* inputA, InstRO::Pass* inputB) { return NULL; }
InstRO::Pass* RosePassFactory::createCallPathSelector(InstRO::Pass* inputA, InstRO::Pass* inputB) { return NULL; }
InstRO::Pass* RosePassFactory::createOpenMPSelector() { return NULL; }
InstRO::Pass* RosePassFactory::createOpenMPFilter(InstRO::Pass* input) { return NULL; }
InstRO::Pass* RosePassFactory::createOpenMPOpariCannonizer(InstRO::Pass* input) { return NULL; }
InstRO::Pass* RosePassFactory::createOPARIAdapter(InstRO::Pass* input) { return NULL; }

}	// Rose
}	// InstRO
