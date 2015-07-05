#include "instro/utility/overheadestimator.h"

#define VERBOSE 1

using namespace InstRO;

void OverheadEstimator::visit(SgNode* node) {
	if (isSgFunctionDefinition(node)) {
		numberOfFunctions++;
		ASTAttributeContainer* container = ASTAttributeContainer::hasASTAttributeContainer(node);
		if (container) {
			if (container->hasSelAttrib(selector->getId())) {
				numberOfFunctionsToInstrument++;
				SgFunctionDefinition* fDef = isSgFunctionDefinition(node);
				SgFunctionDeclaration* fDecl = fDef->get_declaration();
				if (fDecl == NULL) {
					std::cerr << "ERROR: No declaration" << std::endl;
				}
				fNames.push_back(fDecl);
			}
		}
	}
}

void OverheadEstimator::printResult() {
	std::cout << "---- Overhead Estimator ----\n";
	std::cout << "Instrumented Function requires additional cycles: " << additionalCycles;
	std::cout << "\nInstrumented Function requires additional nanos: " << additionalNanos;
	std::cout << std::endl;
	std::cout << "---- AST contains ----\n";
	std::cout << "Overall Functions: " << numberOfFunctions;
	std::cout << "\nNumber of functions to be instrumented: " << numberOfFunctionsToInstrument;
	std::cout << "\n---- Added overhead (estimate) ----\n";
	std::cout << "Additional cycles required: " << approxAdditionalCycles;
	std::cout << "\nAdditional nanoseconds required:" << approxAdditionalNanos << std::endl;
	writeFile();
}

void OverheadEstimator2::estimate() {
	approxAdditionalCycles = 0;
	approxAdditionalNanos = 0;
	numberOfFunctionsToInstrument = 0;
	boost::unordered_map<SgFunctionDefinition*, long long>::iterator iter;
	for (iter = callsByFuncDef.begin(); iter != callsByFuncDef.end(); iter++) {
		if ((*iter).first == NULL)
			continue;
		//		std::cout << "Iterating. \n";
		ASTAttributeContainer* container = ASTAttributeContainer::hasASTAttributeContainer((*iter).first);
		if (container) {
			//			std::cout << "Container.\n";
			if (container->hasSelAttrib(selector->getId())) {
#if VERBOSE > 1
				std::cout << "--- In Function : " << (*iter).first->get_declaration()->get_name() << " ---" << std::endl;
				std::cout << "\tthe count number was: " << (*iter).second << std::endl;
#endif
				//				std::cout << "Attribute." << std::endl;
				fNames.push_back((*iter).first->get_declaration());
				approxAdditionalCycles += (*iter).second * 11;
				approxAdditionalNanos += (*iter).second * 11 / 2.7;
				numberOfFunctionsToInstrument++;
			}
		}
	}
}

#include <sstream>
void OverheadEstimator::writeFile() {
	std::ofstream outFile;
	outFile.open("FunctionsToInstrument.txt", std::ios_base::app);

	std::stringstream ss;

	for (std::vector<SgFunctionDeclaration*>::iterator iter = fNames.begin(); iter != fNames.end(); iter++) {
		ss << (*iter)->get_qualified_name() << "(";
		SgFunctionType* fType = (*iter)->get_type();
		SgTypePtrList args = fType->get_arguments();
		for (SgTypePtrList::iterator typeIter = args.begin(); typeIter != args.end(); typeIter++) {
#if VERBOSE > 0
			std::cerr << "The function is: " << (*iter)->get_qualified_name() << std::endl;
#endif
			SgDeclarationStatement* declStmt = (*typeIter)->getAssociatedDeclaration();
			if (declStmt == NULL) {
				std::cerr << "INSTRO ERROR: declStmt == NULL" << std::endl;
				continue;
			}
			SgSymbol* symbolPtr = declStmt->search_for_symbol_from_symbol_table();
			if (symbolPtr == NULL) {
				std::cerr << "INSTRO ERROR: symbolPtr == NULL" << std::endl;
				continue;
			}
			SgName name = symbolPtr->get_name();

			ss << name;
			if ((typeIter + 1) != args.end())
				ss << ", ";
		}
		ss << ")\n";
	}
	// eliminate " in the string
	std::string outString = ss.str();
	for (size_t curPos = 0; curPos < outString.length(); curPos++) {
		if (outString[curPos] == '"') {
			outString[curPos] = ' ';
		}
	}

	outFile << outString;
}

void OverheadEstimator2::writeFile() {
	std::ofstream outFile;
	outFile.open("FunctionsToInstrument.txt", std::ios_base::app);

	std::stringstream ss;

	for (std::vector<SgFunctionDeclaration*>::iterator iter = fNames.begin(); iter != fNames.end(); iter++) {
		ss << (*iter)->get_qualified_name() << "(";
		SgFunctionType* fType = (*iter)->get_type();
		SgTypePtrList args = fType->get_arguments();
		for (SgTypePtrList::iterator typeIter = args.begin(); typeIter != args.end(); typeIter++) {
#if VERBOSE > 0
			std::cerr << "The function is: " << (*iter)->get_qualified_name() << std::endl;
#endif
			SgDeclarationStatement* declStmt = (*typeIter)->getAssociatedDeclaration();
			if (declStmt == NULL) {
				std::cerr << "INSTRO ERROR: declStmt == NULL" << std::endl;
				continue;
			}
			SgSymbol* symbolPtr = declStmt->search_for_symbol_from_symbol_table();
			if (symbolPtr == NULL) {
				std::cerr << "INSTRO ERROR: symbolPtr == NULL" << std::endl;
				continue;
			}
			SgName name = symbolPtr->get_name();

			ss << name;
			if ((typeIter + 1) != args.end())
				ss << ", ";
		}
		ss << ")\n";
	}
	// eliminate " in the string
	std::string outString = ss.str();
	for (size_t curPos = 0; curPos < outString.length(); curPos++) {
		if (outString[curPos] == '"') {
			outString[curPos] = ' ';
		}
	}

	outFile << outString;
}

void OverheadEstimator2::printResult() {
	std::cout << "---- Overhead Estimator ----\n";
	std::cout << "Instrumented Function requires additional cycles: " << 11;
	std::cout << "\nInstrumented Function requires additional nanos @ 2.7GHz: " << 11 / 2.7;
	std::cout << std::endl;
	std::cout << "---- AST contains ----\n";
	std::cout << "Overall Functions: " << callsByFuncDef.size();
	std::cout << "\nNumber of Functions to be instrumented: " << numberOfFunctionsToInstrument;
	std::cout << "\n---- Added overhead (estimate) ----\n";
	std::cout << "Additional cycles required: " << approxAdditionalCycles;
	std::cout << "\nAdditional nanoseconds required:" << approxAdditionalNanos;
	std::cout << "\nAdditional runtime in mm:ss : " << approxAdditionalNanos / (60 * 1e9) << ":"
						<< approxAdditionalNanos / 1e9 << std::endl;
	writeFile();
}
