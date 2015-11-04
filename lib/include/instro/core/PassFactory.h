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

	virtual Pass* createProgramEntrySelector() = 0;
	/*
	 * According to our minimal InstRO compliance file this is the list of components we need to provide.
	 */
	Pass* createBooleanAndSelector(Pass* passA, Pass* passB);
	Pass* createBooleanOrSelector(Pass* passA, Pass* passB);
	Pass* createBooleanXorSelector(Pass* passA, Pass* passB);
	Pass* createBooleanMinusSelector(Pass* passA, Pass* passB);

	virtual Pass* createIdentifierMatcherSelector(std::vector<std::string> matchList) = 0;
	virtual Pass* createCallpathSelector(Pass* passA, Pass* passB) = 0;
	virtual Pass* createConstructClassSelector(InstRO::Core::ConstructTraitType constructClass) = 0;
	virtual Pass* createAggregationStatementCountSelector(int threshold) = 0;

	virtual Pass* createConstructRaisingElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType level) = 0;
	virtual Pass* createConstructLoweringElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType level) = 0;
	virtual Pass* createConstructCroppingElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType minLevel,
																								InstRO::Core::ConstructTraitType maxLevel) = 0;

	virtual Pass* createDefaultInstrumentationAdapter(Pass* input) = 0;

 protected:
	PassManagement::PassManager* passManager;
};
}
#endif
