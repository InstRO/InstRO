
#include "instro/core/Pass.h"
#include "instro/core/Singleton.h"
#include "instro/pass/selector/IdentifyerSelector.h"
#include "instro/pass/selector/ProgramEntrySelector.h"
#include "instro/pass/selector/CallPathSelector.h"
#include "instro/pass/selector/ConstructClassSelector.h"
#include "instro/rose/pass/adapter/ConstructHierarchyASTDotGenerator.h"
#include "instro/rose/pass/transformer/UniqueCallpathTransformer.h"
#include "instro/pass/selector/ElevatorSelector.h"

#include "instro/rose/RosePassFactory.h"

#include "instro/rose/pass/adapter/RoseStrategyBasedAdapter.h"

#include "rose.h"

namespace InstRO {
namespace Rose {

InstRO::Pass* RosePassFactory::createMatthiasZoellnerLoopInstrumentationAdapter(InstRO::Pass* pass) {
	auto initializer = std::make_shared<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::ScorePInitializer>();
	std::unique_ptr<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy> my_strategy =
			std::make_unique<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::ScorePStatementWrapperStrategy>(initializer);
	std::unique_ptr<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy> my_strategy2 =
			std::make_unique<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::ScorePLoopIterationStrategy>(initializer);
	std::unique_ptr<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy> my_strategy3 =
			std::make_unique<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::ScorePFunctionScopeStrategy>(initializer);

	Pass* newPass = new Pass(new InstRO::Rose::Adapter::RoseStrategyBasedAdapter(
			pass, std::move(my_strategy), std::move(my_strategy2), std::move(my_strategy3)));
	newPass->setPassName("InstRO::Rose::Adapter::MatthiasZoellnerLoopInstrumentationAdapter");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createConstructHierarchyASTDotGenerator(InstRO::Pass* pass, std::string fileName) {
	Pass* newPass = new Pass(new InstRO::Rose::Adapter::RoseConstructHierarchyASTDotGenerator(pass, fileName));
	newPass->setPassName("InstRO::Rose::Adapter::ConstructHierarchyASTDotGenerator.h");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* RosePassFactory::createConstructRaisingElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType level) {
	Pass* newPass = new Pass(new InstRO::Selector::ConstructRaisingElevator(pass, level));
	newPass->setPassName("InstRO::Selector::ConstructRaisingElevator");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* RosePassFactory::createConstructLoweringElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType level) {
	Pass* newPass = new Pass(new InstRO::Selector::ConstructLoweringElevator(pass, level));
	newPass->setPassName("InstRO::Selector::ConstructLoweringElevator");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* RosePassFactory::createConstructCroppingElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType minLevel,
																											 InstRO::Core::ConstructTraitType maxLevel) {
	Pass* newPass = new Pass(new InstRO::Selector::ConstructCroppingElevator(pass, minLevel, maxLevel));
	newPass->setPassName("InstRO::Selector::ConstructCroppingElevator");
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

InstRO::Pass* createFunctionBlackAndWhiteListFilter(std::vector<std::string> rules, Pass* inputPasses) {
	throw std::string("Not yet Implemented");
	return nullptr;
}

/*
Pass* RosePassFactory::createBlackNWhiteSelector(std::string string) {
	std::vector<std::string> filters;
	filters.push_back(string);
	return createBlackAndWhiteListSelector(filters);
};*/

// CI: Todo
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

// CI: Todo
InstRO::Pass* RosePassFactory::createBooleanAndSelector(InstRO::Pass* inputA, InstRO::Pass* inputB) {
	throw std::string("Not yet Implemented");
	return NULL;
}

Pass* RosePassFactory::createProgramEntrySelector() {
	Pass* newPass = new Pass(new InstRO::Selector::ProgramEntrySelector());
	newPass->setPassName("InstRO::Selector::ProgramEntrySelector");
	passManager->registerPass(newPass);
	return newPass;
};

Pass* RosePassFactory::createGenericAdapter(Pass* functionSelection, Pass* loopSelection, Pass* branchingSelection) {
	// RosePass * roseFunctionSelectionPass,* roseLoopSelectionPass,*roseBranchingSelectionPass;
	/*
	Adapter::GenericAdapter* roseAdapter =
	new Adapters::GenericAdapter(functionSelection, loopSelection, branchingSelection);
	Pass* newPass = new Pass(roseAdapter);
	newPass->setPassName("InstRO::Rose::GenericAdapter");
	passManager->registerPass(newPass);*/
	throw std::string("Not yet Implemented");
	return nullptr;
};

InstRO::Pass* RosePassFactory::createGenericAdapter(GenericAdapterConfiguration gac) {
	return createGenericAdapter(gac.getFunctionSelector(), gac.getLoopConstructSelector(), gac.getLoopBodySelector());
};

InstRO::Pass* RosePassFactory::createIdentifyerSelector(std::vector<std::string> matchList) {
	Pass* newPass = new Pass(new InstRO::Selector::IdentifyerSelector(matchList));
	newPass->setPassName("InstRO::Rose::IdentifyerSelector");
	passManager->registerPass(newPass);
	return newPass;
};

InstRO::Pass* RosePassFactory::createIdentifyerFilter(std::vector<std::string> matchList, InstRO::Pass* filterInput) {
	Pass* newPass = new Pass(new InstRO::Selector::IdentifyerSelector(matchList, filterInput));
	newPass->setPassName("InstRO::Rose::IdentifyerFilter");
	passManager->registerPass(newPass);
	return newPass;
};

InstRO::Pass* RosePassFactory::createFunctionSelector() {
	throw std::string("Not yet Implemented");
	return NULL;
}

InstRO::Pass* RosePassFactory::createGPIAdapter(InstRO::Pass* input) {
	throw std::string("Not yet Implemented");
	return NULL;
}

InstRO::Pass* RosePassFactory::createFunctionBlackAndWhiteListFilter(std::vector<std::string> matchList,
																																		 InstRO::Pass* inputB) {
	throw std::string("Not yet Implemented");
	return NULL;
}

InstRO::Pass* RosePassFactory::createCallPathSelector(InstRO::Pass* callee, InstRO::Pass* caller) {
	InstRO::Pass* newPass = new InstRO::Pass(new InstRO::Selectors::CallPathSelector(callee, caller));
	newPass->setPassName("InstRO::Selector::CallPathSelector");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createConstructClassSelector(InstRO::Core::ConstructTraitType constructClass) {
	InstRO::Pass* newPass = new InstRO::Pass(new InstRO::Selector::ConstructClassSelector(constructClass));
	newPass->setPassName("InstRO::Selector::ConstructClassSelector");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createOpenMPSelector() {
	throw std::string("Not yet Implemented");
	return NULL;
}
InstRO::Pass* RosePassFactory::createOpenMPFilter(InstRO::Pass* input) {
	throw std::string("Not yet Implemented");
	return NULL;
}
InstRO::Pass* RosePassFactory::createOpenMPOpariCannonizer(InstRO::Pass* input) {
	throw std::string("Not yet Implemented");
	return NULL;
}
InstRO::Pass* RosePassFactory::createOPARIAdapter(InstRO::Pass* input) {
	throw std::string("Not yet Implemented");
	return NULL;
}

InstRO::Pass* RosePassFactory::createUniqueCallpathTransformer(InstRO::Pass* input) {
	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::UniqueCallpathTransformer(input));
	newPass->setPassName("InstRO::Rose::Transformer::UniqueCallpathTransformer");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createUniqueCallpathTransformer(Pass* input, Pass* root, Pass* active) {
	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::UniqueCallpathTransformer(input, root, active));
	newPass->setPassName("InstRO::Rose::Transformer::UniqueCallpathTransformer");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createFunctionWrapper(
		InstRO::Pass* input, InstRO::Rose::Transformer::FunctionWrapper::NameTransformer nameTransformer) {
	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::FunctionWrapper(input, nameTransformer));
	newPass->setPassName("InstRO::Rose::Transformer::FunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}
InstRO::Pass* RosePassFactory::createFunctionWrapper(
		InstRO::Pass* input, InstRO::Pass* renaming,
		InstRO::Rose::Transformer::FunctionWrapper::NameTransformer nameTransformer, const std::string& definitionPrefix,
		const std::string& wrapperPrefix) {
	InstRO::Pass* newPass = new InstRO::Pass(
			new Transformer::FunctionWrapper(input, renaming, nameTransformer, definitionPrefix, wrapperPrefix));
	newPass->setPassName("InstRO::Rose::Transformer::FunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createMPIFunctionWrapper(InstRO::Pass* input) {
	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::MPIFunctionWrapper(input));
	newPass->setPassName("InstRO::Rose::Transformer::MPIFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createMPIFunctionWrapper(InstRO::Pass* input, InstRO::Pass* renaming,
																												const std::string& definitionPrefix,
																												const std::string& wrapperPrefix) {
	InstRO::Pass* newPass =
			new InstRO::Pass(new Transformer::MPIFunctionWrapper(input, renaming, definitionPrefix, wrapperPrefix));
	newPass->setPassName("InstRO::Rose::Transformer::MPIFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

}	// Rose
}	// InstRO
