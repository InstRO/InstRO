
#include "instro/utility/BWLFileReader.h"
#include <vector>
#include <iostream>

using namespace InstRO;

Utility::BWLFileReader::BWLFileReader(std::string filename) : wSpecifier("+"), bSpecifier("-"), filename(filename){
}

std::pair<std::vector<std::string>, std::vector<std::string> > 
Utility::BWLFileReader::getBWList(){
	if(! hasBeenRead){
		readFile();
	}

	return std::make_pair(blacklist, whitelist);
}

void Utility::BWLFileReader::readFile(){
	if(filename.empty()){
		std::cerr << "InstRO Error: The filename was empty in BWLFileReader\n" << std::endl;
		return;
	}
	std::ifstream inFile;
	inFile.open(filename.c_str());

	std::string tStr;
	while(inFile.good()) {
		
		std::getline(inFile, tStr);

		if(boost::starts_with(tStr, wSpecifier)) {
			whitelist.push_back(prepareName(tStr, wSpecifier));
		} else if(boost::starts_with(tStr, bSpecifier)) {
			blacklist.push_back(prepareName(tStr, bSpecifier));
		}
	}

	inFile.close();
}

std::string Utility::BWLFileReader::prepareName(std::string name, std::string specifier){
	name.erase(0, specifier.length());
	boost::trim(name);

	if(!boost::starts_with("::", name)){
		return "::" + name;
	}

	return name;
}
