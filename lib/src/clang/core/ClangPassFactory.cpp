#include "instro/clang/core/ClangPassFactory.h"

InstRO::Pass *InstRO::Clang::ClangPassFactory::createBlackAndWhiteListSelector(std::vector<std::string> blacklist,
																																							 std::vector<std::string> whitelist) {
	auto pImpl = new InstRO::Clang::BlackWhitelistSelector(blacklist, whitelist);
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(), "InstRO::Clang::BlackWhitelistSelector");
//	p->setPassName("BlackWhitelist Selector");
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::ClangPassFactory::createFunctionDefinitionSelector() {
	auto pImpl = new InstRO::Clang::FunctionDefinitionSelector();
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(), "InstRO::Clang::FunctionDefinitionSelector");
//	p->setPassName(std::string("Function Definition Selector"));
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::ClangPassFactory::createCygProfileAdapter(InstRO::Pass *input) {
	using ConfigTuple = InstRO::Core::ChannelConfiguration::ConfigTuple;
	auto pImpl = new InstRO::Clang::CygProfileAdapter(replacements, nullptr);
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(input), "InstRO::Clang::CygProfileAdapter");
//	p->setPassName(std::string("CygProfile Adapter"));
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::ClangPassFactory::createLLVMInputAdapter(InstRO::Pass *input) {
	using ConfigTuple = InstRO::Core::ChannelConfiguration::ConfigTuple;
	auto pImpl = new InstRO::Clang::LLVMInputAdapter();
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(input), "InstRO::Clang::LLVMInputAdapter");
//	p->setPassName(std::string("LLVM Input Adapter"));
	passManager->registerPass(p);
	return p;
}
