#include "instro/llvm/selector/ClangFrontendSelector.h"

char InstRO::LLVM::ClangFrontendSelector::ID = 0;

InstRO::LLVM::ClangFrontendSelector::ClangFrontendSelector()
		: llvm::FunctionPass(ID), pn("LLVM Input Selector"), filename("instro-temp-file") {
	std::cout << "Constructing ClangFrontendSelector" << std::endl;
	parseInputFile();
}

InstRO::LLVM::ClangFrontendSelector::ClangFrontendSelector(std::string filename)
		: llvm::FunctionPass(ID), pn("LLVM Input Selector"), filename(filename) {
	parseInputFile();
}

bool InstRO::LLVM::ClangFrontendSelector::runOnFunction(llvm::Function &function) {
	std::cout << "Running on function in ClangFrontendSelector" << std::endl;
	for (auto &s : funcsToMark) {
		std::cout << s << std::endl;
	}
	if (funcsToMark.find(function.getName().str()) != funcsToMark.end()) {
		std::cout << "Putting: " << function.getName().str() << std::endl;
		put(&function);
	}
	return false;
}

void InstRO::LLVM::ClangFrontendSelector::parseInputFile() {
	std::ifstream inFile;
	inFile.open(filename.c_str());
	while (inFile.good()) {
		std::string t;
		std::getline(inFile, t);
		auto parts = explode(t);
		if (parts.size() == 3) {
			funcsToMark.insert(parts.at(2));
		}
	}

	inFile.close();
}

std::vector<std::string> InstRO::LLVM::ClangFrontendSelector::explode(std::string s) {
//	std::cout << "In \"explode\": \n" << s << "\n";
	size_t pos = 0;
	std::vector<std::string> rv;
	while (s.find(' ', pos) != std::string::npos) {
		size_t pos2 = s.find(' ', pos);
		std::string t = s.substr(pos, pos2 - pos);
		rv.push_back(t);
		pos = s.find(' ', pos);
		pos++;
	}
	rv.push_back(s.substr(pos));
//	for (auto &ss : rv) {
//		std::cout << ss << "\n";
//	}
//	std::cout << "---\ndone.\nReturning vector of size: " << rv.size() << std::endl;
	return rv;
}
