#ifndef INSTRO_CORE_PASSFACTORY_H
#define INSTRO_CORE_PASSFACTORY_H

#include "instro/core/Pass.h"
#include "instro/core/PassManager.h"

#include <string>

namespace InstRO {
class PassFactory {
 public:
	/* CI: A PassFactory must be initialized with the PassManager. */
	PassFactory(PassManagement::PassManager* refManager) : passManager(refManager){};
	virtual ~PassFactory() {}

	Pass* createProgramEntrySelector();
	Pass* createConstructHierarchyASTDotGenerator(Pass* pass, std::string fileName);

	/*
	 * According to our minimal InstRO compliance file this is the list of components we need to provide.
	 */
	Pass* createBooleanAndSelector(Pass* passA, Pass* passB);
	Pass* createBooleanOrSelector(Pass* passA, Pass* passB);
	Pass* createBooleanXorSelector(Pass* passA, Pass* passB);
	Pass* createBooleanMinusSelector(Pass* passA, Pass* passB);

	Pass* createIdentifierMatcherSelector(std::vector<std::string> matchList);
	Pass* createCallpathSelector(Pass* passA, Pass* passB, std::string dotName = std::string(""));
	Pass* createConstructClassSelector(InstRO::Core::ConstructTraitType constructClass);
	Pass* createAggregationStatementCountSelector(int threshold);

	Pass* createConstructRaisingElevator(Pass* pass, InstRO::Core::ConstructTraitType level);
	Pass* createConstructLoweringElevator(Pass* pass, InstRO::Core::ConstructTraitType level);
	Pass* createConstructCroppingElevator(Pass* pass, InstRO::Core::ConstructTraitType minLevel,
																				InstRO::Core::ConstructTraitType maxLevel);

	virtual Pass* createDefaultInstrumentationAdapter(Pass* input) = 0;

 protected:
	PassManagement::PassManager* passManager;
};
}
#endif
