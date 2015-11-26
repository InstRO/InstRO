#include "instro/core/PassFactory.h"

#include "instro/pass/selector/BooleanCompoundSelector.h"
#include "instro/pass/selector/IdentifierSelector.h"
#include "instro/pass/selector/ProgramEntrySelector.h"
#include "instro/pass/selector/CallPathSelector.h"
#include "instro/pass/selector/ConstructClassSelector.h"
#include "instro/pass/selector/ElevatorSelector.h"

#include <string>

namespace InstRO {

Pass* PassFactory::createBooleanOrSelector(Pass* inputA, Pass* inputB) {
	Pass* newPass = new Pass(
			new Selector::BooleanCompoundSelector(inputA, inputB, Selector::BooleanCompoundSelector::CO_OR));
	newPass->setPassName("InstRO::Selector::BooleanOrSelector");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* PassFactory::createBooleanAndSelector(InstRO::Pass* inputA, InstRO::Pass* inputB) {
	Pass* newPass = new Pass(
			new Selector::BooleanCompoundSelector(inputA, inputB, Selector::BooleanCompoundSelector::CO_AND));
	newPass->setPassName("InstRO::Selector::BooleanAndSelector");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* PassFactory::createBooleanXorSelector(InstRO::Pass* inputA, InstRO::Pass* inputB) {
	Pass* newPass = new Pass(
			new Selector::BooleanCompoundSelector(inputA, inputB, Selector::BooleanCompoundSelector::CO_XOR));
	newPass->setPassName("InstRO::Selector::BooleanXorSelector");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* PassFactory::createBooleanMinusSelector(InstRO::Pass* inputA, InstRO::Pass* inputB) {
	Pass* newPass = new Pass(
			new Selector::BooleanCompoundSelector(inputA, inputB, Selector::BooleanCompoundSelector::CO_MINUS));
	newPass->setPassName("InstRO::Selector::BooleanMinusSelector");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* PassFactory::createConstructRaisingElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType level) {
	Pass* newPass = new Pass(new Selector::ConstructRaisingElevator(pass, level));
	newPass->setPassName("InstRO::Selector::ConstructRaisingElevator");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* PassFactory::createConstructLoweringElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType level) {
	Pass* newPass = new Pass(new Selector::ConstructLoweringElevator(pass, level));
	newPass->setPassName("InstRO::Selector::ConstructLoweringElevator");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* PassFactory::createConstructCroppingElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType minLevel,
																											 InstRO::Core::ConstructTraitType maxLevel) {
	Pass* newPass = new Pass(new Selector::ConstructCroppingElevator(pass, minLevel, maxLevel));
	newPass->setPassName("InstRO::Selector::ConstructCroppingElevator");
	passManager->registerPass(newPass);
	return newPass;
}

Pass* PassFactory::createDefaultInstrumentationAdapter(InstRO::Pass* input) {
	// TODO implement me
	throw std::string("Not yet Implemented");
}

Pass* PassFactory::createProgramEntrySelector() {
	Pass* newPass = new Pass(new Selector::ProgramEntrySelector());
	newPass->setPassName("InstRO::Selector::ProgramEntrySelector");
	passManager->registerPass(newPass);
	return newPass;
};

InstRO::Pass* PassFactory::createIdentifierMatcherSelector(std::vector<std::string> matchList) {
	Pass* newPass = new Pass(new Selector::IdentifyerSelector(matchList));
	newPass->setPassName("InstRO::Rose::IdentifyerSelector");
	passManager->registerPass(newPass);
	return newPass;
};

InstRO::Pass* PassFactory::createCallpathSelector(InstRO::Pass* callee, InstRO::Pass* caller, std::string dotName) {
	Pass* newPass = new InstRO::Pass(new Selector::CallPathSelector(callee, caller, dotName));
	newPass->setPassName("InstRO::Selector::CallPathSelector");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* PassFactory::createConstructClassSelector(InstRO::Core::ConstructTraitType constructClass) {
	Pass* newPass = new InstRO::Pass(new Selector::ConstructClassSelector(constructClass));
	newPass->setPassName("InstRO::Selector::ConstructClassSelector");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* PassFactory::createAggregationStatementCountSelector(int threshold) {
	// TODO implement me
	throw std::string("Not yet Implemented");
}


}	// namespace InstRO
