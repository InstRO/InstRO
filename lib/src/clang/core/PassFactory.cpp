#include "instro/clang/core/PassFactory.h"

InstRO::Pass *InstRO::Clang::PassFactory::createBlackAndWhiteListSelector(std::vector<std::string> blacklist,
																																					std::vector<std::string> whitelist) {
	InstRO::Clang::Core::ClangPassImplementation *pImpl = new InstRO::Clang::BlackWhitelistSelector(blacklist, whitelist);
	pImpl->setPassExecuter(vExecuter);
	InstRO::Pass *p = new InstRO::Pass(pImpl);
	p->setPassName("BlackWhitelist Selector");
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::PassFactory::createBooleanOrSelector(InstRO::Pass *inputA, InstRO::Pass *inputB) {
	InstRO::Clang::Core::ClangPassImplementation *pImpl =
			new InstRO::Clang::BooleanCompoundSelector(inputA, inputB, InstRO::Clang::BooleanCompoundSelector::OR);
	pImpl->setPassExecuter(nvExecuter);
	InstRO::Pass *p = new InstRO::Pass(pImpl);
	p->setPassName("Boolean OR Selector");
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::PassFactory::createFunctionDefinitionSelector() {
	InstRO::Clang::Core::ClangPassImplementation *pImpl = new InstRO::Clang::FunctionDefinitionSelector();
	pImpl->setPassExecuter(vExecuter);
	InstRO::Pass *p = new InstRO::Pass(pImpl);
	p->setPassName(std::string("Function Definition Selector"));
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::PassFactory::createCygProfileAdapter(InstRO::Pass *input) {
	InstRO::Core::ChannelConfiguration cc(input);
	InstRO::Clang::Core::ClangPassImplementation *pImpl = new InstRO::Clang::CygProfileAdapter(cc, replacements, NULL);
	pImpl->setPassExecuter(vExecuter);
	InstRO::Pass *p = new InstRO::Pass(pImpl);
	p->setPassName(std::string("CygProfile Adapter"));
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::PassFactory::createLLVMInputAdapter(InstRO::Pass *input) {
	InstRO::Core::ChannelConfiguration cc(input);
	InstRO::Clang::Core::ClangPassImplementation *pImpl = new InstRO::Clang::LLVMInputAdapter(cc);
	pImpl->setPassExecuter(nvExecuter);
	InstRO::Pass *p = new InstRO::Pass(pImpl);
	p->setPassName(std::string("LLVM Input Adapter"));
	passManager->registerPass(p);
	return p;
}
