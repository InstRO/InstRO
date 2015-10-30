#include "instro/clang/core/PassFactory.h"

InstRO::Pass *InstRO::Clang::PassFactory::createBlackAndWhiteListSelector(std::vector<std::string> blacklist,
																																					std::vector<std::string> whitelist) {
	auto pImpl = new InstRO::Clang::BlackWhitelistSelector(blacklist, whitelist);
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl);
	p->setPassName("BlackWhitelist Selector");
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::PassFactory::createBooleanOrSelector(InstRO::Pass *inputA, InstRO::Pass *inputB) {
	auto pImpl = new InstRO::Clang::BooleanCompoundSelector(inputA, inputB, InstRO::Clang::BooleanCompoundSelector::OR);
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl);
	p->setPassName("Boolean OR Selector");
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::PassFactory::createFunctionDefinitionSelector() {
	auto pImpl = new InstRO::Clang::FunctionDefinitionSelector();
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl);
	p->setPassName(std::string("Function Definition Selector"));
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::PassFactory::createCygProfileAdapter(InstRO::Pass *input) {
	InstRO::Core::ChannelConfiguration cc(input);
	auto pImpl = new InstRO::Clang::CygProfileAdapter(cc, replacements, nullptr);
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl);
	p->setPassName(std::string("CygProfile Adapter"));
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::PassFactory::createLLVMInputAdapter(InstRO::Pass *input) {
	InstRO::Core::ChannelConfiguration cc(input);
	auto pImpl = new InstRO::Clang::LLVMInputAdapter(cc);
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl);
	p->setPassName(std::string("LLVM Input Adapter"));
	passManager->registerPass(p);
	return p;
}
