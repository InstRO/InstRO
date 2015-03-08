#include "instro/clang/core/PassFactory.h"

InstRO::Pass* InstRO::Clang::PassFactory::createBlackAndWhiteListSelector(
		std::vector<std::string> rules) {
	return NULL;
}

InstRO::Pass* InstRO::Clang::PassFactory::createBooleanOrSelector(
		InstRO::Pass* inputA, InstRO::Pass* inputB) {
	return NULL;
}

InstRO::Pass* InstRO::Clang::PassFactory::createFunctionDefinitionSelector() {
	InstRO::Pass *p = new InstRO::Pass(new InstRO::Clang::FunctionDefinitionSelector());
	passManager->registerPass(p);
	return p;
}

InstRO::Pass* InstRO::Clang::PassFactory::createCygProfileAdapter(
		InstRO::Pass* input) {
	InstRO::Pass *p = new InstRO::Pass(new InstRO::Clang::CygProfileAdapter(input, NULL));
	passManager->registerPass(p);
	return p;
}

