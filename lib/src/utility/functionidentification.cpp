#include "instro/utility/functionidentification.h"

using namespace InstRO;

FunctionIdentificationTable::FunctionIdentificationTable(){
	uniqueLabel = 1;
	fileCorrupt = false;
	if(checkIfNMFileExists())
			uniqueLabel = getFirstFreeIdentifier();

#ifdef DEBUG
	std::cout << "uniqueLabel initialized to: " << uniqueLabel << std::endl;
#endif
	labelToFunctionName = std::map<unsigned long, std::string>();
	writeCalled = false;
}

unsigned long FunctionIdentificationTable::insertIdentifier(SgFunctionDefinition* funcDef){
	uniqueLabel+=8;
	labelToFunctionName.insert(std::pair<unsigned long, std::string>(uniqueLabel, generateNamefileEntryName(funcDef)));
}

void FunctionIdentificationTable::writeNamefile() {
	std::ofstream out;
	if(!fileCorrupt){
		out.open("InstRO_nmfile", std::ios_base::app);
	} else {
		out.open("InstRO_nmfile", std::ios_base::out);
//	out.setf(std::ios_base::showbase);
	}

	std::map<unsigned long, std::string>::iterator i;
	for (i = labelToFunctionName.begin(); i != labelToFunctionName.end(); i++) {
		out << std::setw(16) << std::setfill('0') << std::hex << std::noshowbase << i->first;
		out << " U ";
		out << i->second;
		out << "\n";
	}
	writeCalled = true;
	out.close();
}

unsigned long FunctionIdentificationTable::getLabel(){
	return uniqueLabel;
}

std::string FunctionIdentificationTable::generateNamefileEntryName(SgFunctionDefinition* functionDefinition) {
	SgFunctionType* functionType = functionDefinition->get_declaration()->get_type();
	SgTypePtrList argumentTypes = functionType->get_arguments();
	std::stringstream ss("");
	// 2014-01-02 JP: XXX For instantiated templates the unparseToString method produces
	// really ugly output!
	ss << functionType->get_return_type()->unparseToString() << " ";
	std::string fName = functionDefinition->get_declaration()->get_qualified_name();
	size_t pos;

//	std::cout << fName << std::endl;
	while ((pos = fName.find('"')) != std::string::npos) {
		fName.replace(pos, 1, " ");
	}
	ss << fName;
	ss << "(";
	for (SgTypePtrList::iterator i = argumentTypes.begin(); i != argumentTypes.end(); i++) {
		if (i != argumentTypes.begin()) {
			ss << ", ";
		}
		ss << (*i)->unparseToString();
	}
	ss << ")";
#ifdef DEBUG
	std::cout << "In VampirTraceAdapterBuilder: Built name for name lookup file: " << ss.str() << std::endl;
#endif
	return ss.str();
}

inline void FunctionIdentificationTable::incrementUniqueLabel() {
	this->uniqueLabel += 8;
}
bool FunctionIdentificationTable::checkIfNMFileExists(){
	std::ifstream inFile("InstRO_nmfile");
	if(inFile.is_open())
		return true;
	else
		return false;
}

unsigned long FunctionIdentificationTable::getFirstFreeIdentifier(){
	// Open the file
	std::ifstream inFile("InstRO_nmfile");

	// Check for an empty/invalid file
	inFile.seekg(0, inFile.end);
	if(inFile.tellg() <= 13){
		inFile.close();
		fileCorrupt = true;
		return 1;
	}

	inFile.seekg(-2, inFile.end); // assumes a non-empty file
	char peekChar = inFile.peek();
	std::cout << "Peekchar before loop: " << peekChar << std::endl;
	while(true){
		if(peekChar == '\n' || inFile.tellg() == 0)
			break;

		inFile.seekg(-1, inFile.cur);
		peekChar = inFile.peek();
#ifdef DEBUG
		std::cout << "Peekchar: " << peekChar << std::endl;
#endif
	}
	if(inFile.tellg() > 0)
		inFile.seekg(1, inFile.cur);
	unsigned long extractedNumber;
	inFile >> std::hex >> extractedNumber;
#ifdef DEBUG
	std::cout << "The number read was: " << extractedNumber << std::endl;
#endif
	return extractedNumber;
}
