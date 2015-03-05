#include "instro/clang/core/PassFactory.h"

InstRO::Pass* ::InstRO::Clang::PassFactory::
		createBlackAndWhiteListSelector(std::vector<std::string> rules) {
	return NULL;
}

InstRO::Pass* ::InstRO::Clang::PassFactory::createBooleanOrSelector(
		::InstRO::Pass* inputA, ::InstRO::Pass* inputB) {
	return NULL;
}

InstRO::Pass* ::InstRO::Clang::PassFactory::
		createFunctionDefinitionSelector() {
	return new InstRO::Pass(new InstRO::Clang::FunctionDefinitionSelector());
}

InstRO::Pass* ::InstRO::Clang::PassFactory::createCygProfileAdapter(
		::InstRO::Pass* input) {
	return NULL;
}

