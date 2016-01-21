#include "instro/clang/selector/BlackWhitelistSelector.h"

#include "instro/clang/core/ClangConstruct.h"

InstRO::Clang::BlackWhitelistSelector::BlackWhitelistSelector(std::vector<std::string> blacklist,
																															std::vector<std::string> whitelist)
		: ClangPassImplBase<BlackWhitelistSelector>(new InstRO::Clang::VisitingPassExecuter<BlackWhitelistSelector>()),
			blacklist(blacklist),
			whitelist(whitelist) {
	std::cout << "Creating BW Selector with blacklist(-) and whitelist (+):\n";
	for (auto &s : blacklist) {
		std::cout << "- " << s << "\n";
	}
	for (auto &s : whitelist) {
		std::cout << "+ " << s << "\n";
	}
	std::cout << std::endl;
}

bool InstRO::Clang::BlackWhitelistSelector::VisitFunctionDecl(clang::FunctionDecl *decl) {
	/*
	 * We match the black and white list entries against the function name
	 */
	if (decl->hasBody()) {
		if (decl->doesThisDeclarationHaveABody()) {
			std::cout << "Testing " << decl->getNameInfo().getAsString() << " whether it is black or white listed."
								<< std::endl;
			// This is "picker prefer whitelist"
			if ((isOnList(decl->getNameInfo().getAsString(), whitelist)) ||
					(!isOnList(decl->getNameInfo().getAsString(), blacklist))) {
				std::cout << "BW Selector: Selecting node" << std::endl;
				// select node
				InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci(&outputSet);
				csci.put(std::make_shared<InstRO::Clang::Core::ClangConstruct>(decl));
			}
		}
	}
	return true;
}

void InstRO::Clang::BlackWhitelistSelector::readFilterFile(std::string filename) {
	Utility::BWLFileReader reader(filename);
	auto lists = reader.getBWList();
	blacklist = lists.first;
	whitelist = lists.second;
}

bool InstRO::Clang::BlackWhitelistSelector::isOnList(std::string functionName, std::vector<std::string> &list) {
	return std::find(list.begin(), list.end(), functionName) != list.end();
}

void InstRO::Clang::BlackWhitelistSelector::addBlacklistEntry(std::string functionName) {
	blacklist.push_back(functionName);
}

void InstRO::Clang::BlackWhitelistSelector::addWhitelistEntry(std::string functionName) {
	whitelist.push_back(functionName);
}
