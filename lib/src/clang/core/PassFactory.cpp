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
	p->setRequiresInput(false);
	p->setProvidesOutput(true);
	p->setPassName(std::string("Function Definition Selector"));
	p->setOutputLevel(InstRO::Core::ConstructLevelType::ConstructLevelStatement);
	passManager->registerPass(p);
	return p;
}

InstRO::Pass* InstRO::Clang::PassFactory::createCygProfileAdapter(
		InstRO::Pass* input) {
	InstRO::Pass *p = new InstRO::Pass(new InstRO::Clang::CygProfileAdapter(input, replacements, NULL));
	p->setRequiresInput(true);
	p->setProvidesOutput(false);
	p->setPassName(std::string("CygProfile Adapter"));
	p->registerInputPass(input, InstRO::Core::ConstructLevelType::ConstructLevelStatement);
	passManager->registerPass(p);
	return p;
}

