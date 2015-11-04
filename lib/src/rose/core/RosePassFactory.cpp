#include "instro/rose/RosePassFactory.h"

#include "instro/pass/selector/IdentifyerSelector.h"
#include "instro/pass/selector/ProgramEntrySelector.h"
#include "instro/pass/selector/CallPathSelector.h"
#include "instro/pass/selector/ConstructClassSelector.h"
#include "instro/pass/selector/ElevatorSelector.h"
#include "instro/rose/pass/transformer/UniqueCallpathTransformer.h"
#include "instro/rose/pass/adapter/ConstructHierarchyASTDotGenerator.h"
#include "instro/rose/pass/adapter/RoseStrategyBasedAdapter.h"
#include "instro/rose/pass/adapter/RoseConstructPrinter.h"

namespace InstRO {
namespace Rose {

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

Pass* RosePassFactory::createInstROMeasurementInterfaceAdapter(InstRO::Pass* input) {
	// TODO implement me
	throw std::string("Not yet Implemented");
}

Pass* RosePassFactory::createConstructPrinter(InstRO::Pass* pass) {
	Pass* newPass = new Pass(new InstRO::Rose::Adapter::RoseConstructPrinter(pass));
	newPass->setPassName("InstRO::Rose::Adapter::RoseConstructPrinter");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* RosePassFactory::createProgramEntrySelector() {
	Pass* newPass = new Pass(new InstRO::Selector::ProgramEntrySelector());
	newPass->setPassName("InstRO::Selector::ProgramEntrySelector");
	passManager->registerPass(newPass);
	return newPass;
};

InstRO::Pass* RosePassFactory::createIdentifierMatcherSelector(std::vector<std::string> matchList) {
	Pass* newPass = new Pass(new InstRO::Selector::IdentifyerSelector(matchList));
	newPass->setPassName("InstRO::Rose::IdentifyerSelector");
	passManager->registerPass(newPass);
	return newPass;
};

InstRO::Pass* RosePassFactory::createCallpathSelector(InstRO::Pass* callee, InstRO::Pass* caller) {
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

InstRO::Pass* RosePassFactory::createAggregationStatementCountSelector(int threshold) {
	// TODO implement me
	throw std::string("Not yet Implemented");
}


/* ROSE ONLY */

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
InstRO::Pass* RosePassFactory::createFunctionWrapper(InstRO::Pass* input, InstRO::Pass* renaming,
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
