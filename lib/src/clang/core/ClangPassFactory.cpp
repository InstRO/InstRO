#include "instro/clang/core/ClangPassFactory.h"

#include "instro/clang/pass/selector/ClangBlackWhitelistSelector.h"

#include "instro/clang/pass/adapter/ClangCygProfileAdapter.h"
#include "instro/clang/pass/adapter/LLVMInputAdapter.h"

InstRO::Pass *InstRO::Clang::ClangPassFactory::createClangBlackAndWhiteListSelector(std::vector<std::string> blacklist,
																																							 std::vector<std::string> whitelist) {
	auto pImpl = new InstRO::Clang::Selector::ClangBlackWhitelistSelector(blacklist, whitelist);
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(), "InstRO::Clang::BlackWhitelistSelector");
//	p->setPassName("BlackWhitelist Selector");
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::ClangPassFactory::createClangCygProfileAdapter(InstRO::Pass *input) {
	using ConfigTuple = InstRO::Core::ChannelConfiguration::ConfigTuple;
	auto pImpl = new InstRO::Clang::Adapter::ClangCygProfileAdapter(replacements, nullptr);
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(input), "InstRO::Clang::CygProfileAdapter");
//	p->setPassName(std::string("CygProfile Adapter"));
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::ClangPassFactory::createLLVMInputAdapter(InstRO::Pass *input) {
	using ConfigTuple = InstRO::Core::ChannelConfiguration::ConfigTuple;
	auto pImpl = new InstRO::Clang::Adapter::LLVMInputAdapter();
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(input), "InstRO::Clang::LLVMInputAdapter");
//	p->setPassName(std::string("LLVM Input Adapter"));
	passManager->registerPass(p);
	return p;
}
