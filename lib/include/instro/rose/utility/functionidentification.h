#ifndef FUNCTION_IDENTIFICATION_HEADER
#define FUNCTION_IDENTIFICATION_HEADER

#include <fstream>
#include <iostream>

#include "rose.h"

namespace InstRO {

/**
 * This table maps an unsigned long to a std::string.
 * At the moment it only handles SgFunctionDefinition nodes.
 * It can only write its internal state as an nmfile to disk.
 *
 * This is class is meant to be used during instrumentation to fulfill identifier handling.
 *
 *\todo add support to not use the consecutive labeling but some sort of hashing (full-qualified-name+parameter)
 *
 * \author Jan-Patrick Lehr
 */
class FunctionIdentificationTable {
 public:
	FunctionIdentificationTable();
	// We only need to get the current label
	unsigned long getLabel();
	unsigned long insertIdentifier(SgFunctionDefinition* funcDef);

	/** write the function identification table as an nmfile to disk*/
	void writeNamefile();

 private:
	unsigned long uniqueLabel;	// holds our pseudo function label

	std::map<unsigned long, std::string>
			labelToFunctionName;	// Holds a mapping from pseudo function ids to function names
	bool writeCalled;					// internally used marker

	void incrementUniqueLabel();
	std::string generateNamefileEntryName(SgFunctionDefinition* functionDefinition);
	bool checkIfNMFileExists();
	unsigned long getFirstFreeIdentifier();
	bool fileCorrupt;
};
}
#endif
