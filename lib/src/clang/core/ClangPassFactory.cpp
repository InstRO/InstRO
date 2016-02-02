#include "instro/clang/ClangPassFactory.h"

#include "instro/clang/pass/selector/ClangBlackWhitelistSelector.h"

#include "instro/clang/pass/adapter/ClangCygProfileAdapter.h"
#include "instro/clang/pass/adapter/ClangMangledNameOutputAdapter.h"

InstRO::Pass *InstRO::Clang::ClangPassFactory::createClangBlackAndWhiteListSelector(std::vector<std::string> blacklist,
																																							 std::vector<std::string> whitelist) {
	auto pImpl = new InstRO::Clang::Selector::ClangBlackWhitelistSelector(blacklist, whitelist);
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(), "InstRO::Clang::ClangBlackWhitelistSelector");
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::ClangPassFactory::createClangCygProfileAdapter(InstRO::Pass *input) {
	using ConfigTuple = InstRO::Core::ChannelConfiguration::ConfigTuple;
	auto pImpl = new InstRO::Clang::Adapter::ClangCygProfileAdapter(replacements, nullptr);
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(input), "InstRO::Clang::ClangCygProfileAdapter");
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *InstRO::Clang::ClangPassFactory::createClangMangledNameOutputAdapter(InstRO::Pass *input) {
	using ConfigTuple = InstRO::Core::ChannelConfiguration::ConfigTuple;
	auto pImpl = new InstRO::Clang::Adapter::ClangMangledNameOutputAdapter();
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(input), "InstRO::Clang::ClangMangledNameOutputAdapter");
	passManager->registerPass(p);
	return p;
}
