#ifndef INSTRO_CORE_FACTORY_H
#define INSTRO_CORE_FACTORY_H

#include "instro/core/Pass.h"
#include "instro/core/PassManager.h"

namespace InstRO {
class PassFactory {
 public:
	/* CI: A PassFactory must be initialized with the PassManager. */
	PassFactory(PassManagement::PassManager* refManager) : passManager(refManager){};
	virtual ~PassFactory() {}

	virtual Pass* createProgramEntrySelector();
	/*
	 * According to our minimal InstRO compliance file this is the list of components we need to provide.
	 */
	Pass* createBooleanAndSelector(Pass* passA, Pass* passB);
	Pass* createBooleanOrSelector(Pass* passA, Pass* passB);
	Pass* createBooleanXorSelector(Pass* passA, Pass* passB);
	Pass* createBooleanMinusSelector(Pass* passA, Pass* passB);

	Pass* createIdentifierMatcherSelector(std::vector<std::string> matchList);
	Pass* createCallpathSelector(Pass* passA, Pass* passB);
	Pass* createConstructClassSelector(InstRO::Core::ConstructTraitType constructClass);
	Pass* createAggregationStatementCountSelector(int threshold);

	Pass* createConstructRaisingElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType level);
	Pass* createConstructLoweringElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType level);
	Pass* createConstructCroppingElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType minLevel,
																								InstRO::Core::ConstructTraitType maxLevel);

	Pass* createDefaultInstrumentationAdapter(Pass* input);

 protected:
	PassManagement::PassManager* passManager;
};
}
#endif
