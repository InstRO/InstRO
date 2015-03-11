#include "BlackWhitelistSelector.h"


using namespace InstRO;

bool BlackWhitelistSelector::VisitFunctionDecl(clang::FunctionDecl *decl){
	/*
	 * We match the black and white list entries against the function name
	 */
	if(decl->hasBody()){
		if(decl->doesThisDieclarationHaveABody()){
			// This is "picker prefer whitelist"
			if((isOnList(decl->getAsString(), whitelist)) || (! isOnList(decl->getNameAsString(), blacklist))){
				std::cout << "Selecting node" << std::endl;
				// select node
				cs.put(decl);
			}
		}
	}
	return true;
}

void BlackWhitelistSelector::readFilterFile(std::string filename){
	util::BWLFileReader reader(filename);
	auto lists = reader.getBWList();
	blacklist = lists.first;
	whitelist = lists.second;
}

bool BlackWhitelistSelector::isOnList(std::string functionName, std::vector<std::string> &list){
	return std::find(list.begin(), list.end(), functionName) == list.end();
}

void BlackWhitelistSelector::addBlacklistEntry(std::string functionName){
	blacklist.push_back(functionName);
}

void BlackWhitelistSelector::addWhitelistEntry(std::string functionName){
	whitelist.push_back(functionName);
}
