#include "instro/clang/ClangPassFactory.h"

#include "instro/clang/core/ClangPassImplBase.h"	// ASTContextProvider

#include "instro/clang/pass/selector/ClangBlackWhitelistSelector.h"

#include "instro/clang/pass/adapter/ClangCygProfileAdapter.h"
#include "instro/clang/pass/adapter/ClangMangledNameOutputAdapter.h"

using namespace InstRO::Clang;

void ClangPassFactory::finishConstruction(clang::ASTContext *context) {
	for (auto p : lazyContextProvidingMap) {
		p->setASTContext(context);
	}
}

InstRO::Pass *ClangPassFactory::createClangBlackAndWhiteListSelector(std::vector<std::string> blacklist,
																																		 std::vector<std::string> whitelist) {
	auto pImpl = new Selector::ClangBlackWhitelistSelector(blacklist, whitelist);
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p =
			new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(), "InstRO::Clang::ClangBlackWhitelistSelector");
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *ClangPassFactory::createClangCygProfileAdapter(InstRO::Pass *input) {
	using ConfigTuple = InstRO::Core::ChannelConfiguration::ConfigTuple;
	auto pImpl = new Adapter::ClangCygProfileAdapter(replacements, nullptr);
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p =
			new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(input), "InstRO::Clang::ClangCygProfileAdapter");
	passManager->registerPass(p);
	return p;
}

InstRO::Pass *ClangPassFactory::createClangMangledNameOutputAdapter(InstRO::Pass *input) {
	using ConfigTuple = InstRO::Core::ChannelConfiguration::ConfigTuple;
	auto pImpl = new Adapter::ClangMangledNameOutputAdapter();
	lazyContextProvidingMap.insert(pImpl);
	InstRO::Pass *p = new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(input),
																		 "InstRO::Clang::ClangMangledNameOutputAdapter");
	passManager->registerPass(p);
	return p;
}
