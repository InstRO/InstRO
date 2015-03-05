#include "instro/clang/core/PassFactory.h"

::InstRO::Clang::Pass* ::InstRO::Clang::PassFactory::
		createBlackAndWhiteListSelector(std::vector<std::string> rules) {
	return NULL;
}

::InstRO::Clang::Pass* ::InstRO::Clang::PassFactory::createBooleanOrSelector(
		::InstRO::Pass* inputA, ::InstRO::Pass* inputB) {
	return NULL;
}

::InstRO::Clang::Pass* ::InstRO::Clang::PassFactory::
		createFunctionDefinitionSelector() {
	return new ::InstRO::Clang::FunctionDefinitionSelector();
}

::InstRO::Clang::Pass* ::InstRO::Clang::PassFactory::createCygProfileAdapter(
		::InstRO::Pass* input) {
	return NULL;
}

