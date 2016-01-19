#include "instro/core/PassFactory.h"

#include "instro/pass/selector/BooleanCompoundSelector.h"
#include "instro/pass/selector/IdentifierSelector.h"
#include "instro/pass/selector/ProgramEntrySelector.h"
#include "instro/pass/selector/CallPathSelector.h"
#include "instro/pass/selector/ConstructClassSelector.h"
#include "instro/pass/selector/ElevatorSelector.h"

#include "instro/pass/adapter/ConstructHierarchyASTDotGenerator.h"
#include "instro/pass/adapter/ConstructPrinterAdapter.h"

#include <string>

namespace InstRO {

Pass* PassFactory::createBooleanOrSelector(Pass* inputA, Pass* inputB) {
	Pass* newPass =
			new Pass(new Selector::BooleanCompoundSelector(Selector::BooleanCompoundSelector::CO_OR), InstRO::Core::ChannelConfiguration(inputA, inputB));
	newPass->setPassName("InstRO::Selector::BooleanOrSelector");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* PassFactory::createBooleanAndSelector(Pass* inputA, Pass* inputB) {
	Pass* newPass =
			new Pass(new Selector::BooleanCompoundSelector(Selector::BooleanCompoundSelector::CO_AND), InstRO::Core::ChannelConfiguration(inputA, inputB));
	newPass->setPassName("InstRO::Selector::BooleanAndSelector");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* PassFactory::createBooleanXorSelector(Pass* inputA, Pass* inputB) {
	Pass* newPass =
			new Pass(new Selector::BooleanCompoundSelector(Selector::BooleanCompoundSelector::CO_XOR), InstRO::Core::ChannelConfiguration(inputA, inputB));
	newPass->setPassName("InstRO::Selector::BooleanXorSelector");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* PassFactory::createBooleanMinusSelector(Pass* inputA, Pass* inputB) {
	Pass* newPass =
			new Pass(new Selector::BooleanCompoundSelector(Selector::BooleanCompoundSelector::CO_MINUS), InstRO::Core::ChannelConfiguration(inputA, inputB));
	newPass->setPassName("InstRO::Selector::BooleanMinusSelector");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* PassFactory::createConstructRaisingElevator(Pass* pass, InstRO::Core::ConstructTraitType level) {
	Pass* newPass = new Pass(new Selector::ConstructRaisingElevator(level), InstRO::Core::ChannelConfiguration(pass));
	newPass->setPassName("InstRO::Selector::ConstructRaisingElevator");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* PassFactory::createConstructLoweringElevator(Pass* pass, InstRO::Core::ConstructTraitType level) {
	Pass* newPass = new Pass(new Selector::ConstructLoweringElevator(level), InstRO::Core::ChannelConfiguration(pass));
	newPass->setPassName("InstRO::Selector::ConstructLoweringElevator");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* PassFactory::createConstructCroppingElevator(Pass* pass, InstRO::Core::ConstructTraitType minLevel,
																									 InstRO::Core::ConstructTraitType maxLevel) {
	Pass* newPass = new Pass(new Selector::ConstructCroppingElevator(minLevel, maxLevel), InstRO::Core::ChannelConfiguration(pass));
	newPass->setPassName("InstRO::Selector::ConstructCroppingElevator");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* PassFactory::createProgramEntrySelector() {
	Pass* newPass = new Pass(new Selector::ProgramEntrySelector(), InstRO::Core::ChannelConfiguration());
	newPass->setPassName("InstRO::Selector::ProgramEntrySelector");
	passManager->registerPass(newPass);
	return newPass;
};

Pass* PassFactory::createIdentifierMatcherSelector(std::vector<std::string> matchList) {
	Pass* newPass = new Pass(new Selector::IdentifyerSelector(matchList), InstRO::Core::ChannelConfiguration());
	newPass->setPassName("InstRO::Rose::IdentifyerSelector");
	passManager->registerPass(newPass);
	return newPass;
};

Pass* PassFactory::createCallpathSelector(Pass* callee, Pass* caller, std::string dotName) {
	Pass* newPass = new Pass(new Selector::CallPathSelector(dotName), InstRO::Core::ChannelConfiguration(callee, caller));
	newPass->setPassName("InstRO::Selector::CallPathSelector");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* PassFactory::createConstructClassSelector(InstRO::Core::ConstructTraitType constructClass) {
	Pass* newPass = new Pass(new Selector::ConstructClassSelector(constructClass), InstRO::Core::ChannelConfiguration());
	newPass->setPassName("InstRO::Selector::ConstructClassSelector");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* PassFactory::createAggregationStatementCountSelector(int threshold) {
	// TODO implement me
	throw std::string("Not yet Implemented");
}

Pass* PassFactory::createConstructHierarchyASTDotGenerator(Pass* pass, std::string fileName) {
	Pass* newPass = new Pass(new InstRO::Adapter::ConstructHierarchyASTDotGenerator(fileName), InstRO::Core::ChannelConfiguration(pass));
	newPass->setPassName("InstRO::Adapter::ConstructHierarchyASTDotGenerator");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* PassFactory::createConstructPrinterAdapter(Pass* pass) {
	Pass* newPass = new Pass(new InstRO::Adapter::ConstructPrinterAdapter(), InstRO::Core::ChannelConfiguration(pass));
	newPass->setPassName("InstRO::Adapter::ConstructPrinterAdapter");
	passManager->registerPass(newPass);
	return newPass;
}

}	// namespace InstRO
