#ifndef INSTRO_CORE_FACTORY_H
#define INSTRO_CORE_FACTORY_H

#include <string>
#include <iostream>

#include <vector>
#include <hash_map>

#include "instro/core/Pass.h"
#include "instro/core/PassManager.h"

namespace InstRO {
/* PassFactory: Interface for the mandatory InstRO Passes. */
class PassFactory {
 public:
	/* CI: A PassFactory must be initialized with the PassManager. */
	PassFactory(PassManagement::PassManager* refManager) : passManager(refManager){};
	virtual Pass* createBlackNWhiteSelector(std::string string) = 0;
	virtual Pass* createProgramEntrySelector() = 0;
	virtual Pass* createCygProfileAdapter(Pass* input) = 0;


	/*
	 * According to our minimal InstRO compliance file this is the list of components we need to provide.
	 */
	virtual Pass* createBooleanAndSelector(Pass *passA, Pass *passB) = 0;
	virtual Pass* createBooleanOrSelector(Pass *passA, Pass *passB) = 0;
	virtual Pass* createIdentifierMatcherSelector() = 0;
	virtual Pass* createCallpathSelector() = 0;
	virtual Pass* createConstructClassSelector() = 0;
	virtual Pass* createAggregationStatementCountSelector() = 0;

	virtual Pass* createConstructRaisingElevator() = 0;
	virtual Pass* createConstructLoweringElevator() = 0;
	virtual Pass* createConstructCroppingElevator() = 0;


	virtual Pass* createInstROMeasurementInterfaceAdapter(Pass *input) = 0;

 protected:
	PassManagement::PassManager* passManager;
};
}
#endif
