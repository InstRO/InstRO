#include "instro/core/PassFactory.h"

#include "instro/pass/selector/BooleanCompoundSelector.h"

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

}	// namespace InstRO
