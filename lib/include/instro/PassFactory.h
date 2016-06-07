#ifndef INSTRO_CORE_PASSFACTORY_H
#define INSTRO_CORE_PASSFACTORY_H

//#include "instro/core/PassManager.h"
#include "instro/core/ConstructTraitType.h"
#include <string>
#include <vector>

namespace InstRO {
namespace PassManagement{
	class PassManager;
}
class Pass;

/**
 * Defines a set of features that every InstRO Implementation should support.
 *
 * Specific implementations are free to offer additional or more convenient components.
 */
class PassFactory {
 public:
	/**
	 * A PassFactory must be initialized with the PassManager to register the created passes.
	 */
	PassFactory(PassManagement::PassManager* refManager) : passManager(refManager){};
	virtual ~PassFactory() {}

	Pass* createProgramEntrySelector();
	Pass* createConstructHierarchyASTDotGenerator(Pass* input, std::string fileName);
	Pass* createConstructPrinterAdapter(Pass* input);

	/*
	 * According to our minimal InstRO compliance file this is the list of components we need to provide.
	 */
	Pass* createBooleanAndSelector(Pass* passA, Pass* passB);
	Pass* createBooleanOrSelector(Pass* passA, Pass* passB);
	Pass* createBooleanXorSelector(Pass* passA, Pass* passB);
	Pass* createBooleanMinusSelector(Pass* passA, Pass* passB);

	Pass* createIdentifierMatcherSelector(std::vector<std::string> matchList);
	Pass* createCallpathSelector(Pass* source, Pass* dest, std::string dotName = std::string(""));
	Pass* createConstructTraitSelector(InstRO::Core::ConstructTraitType constructTrait);

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
