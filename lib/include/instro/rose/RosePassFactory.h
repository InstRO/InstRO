#ifndef INSTRO_ROSE_PASSFACTORY_H
#define INSTRO_ROSE_PASSFACTORY_H

#include "instro/core/PassFactory.h"
#include "instro/core/PassManager.h"

#include "instro/rose/core/RosePassImplementation.h"
// #include "instro/rose/selectors/BlackAndWhiteListSelector.h"
#include "instro/rose/pass/selector/NameBasedSelector.h"
#include "instro/rose/pass/selector/CompoundSelector.h"
// #include "instro/rose/pass/adapter/GenericAdapter.h"
// #include "instro/rose/pass/adapter/CygProfileAdapter.h"
#include "instro/rose/pass/adapter/RoseConstructPrinter.h"
#include "instro/rose/pass/adapter/ConstructHierarchyASTDotGenerator.h"

#include "rose.h"

namespace InstRO {
namespace Rose {

class RosePassFactory : public InstRO::PassFactory {
 public:
	RosePassFactory(PassManagement::PassManager* refManager, SgProject* proj) : PassFactory(refManager), project(proj) {}
	virtual ~RosePassFactory() {}

 protected:
	RosePassImplementation* getPass(Pass* pass) {
		if (pass == NULL)
			return NULL;
		RosePassImplementation* rosePass = dynamic_cast<RosePassImplementation*>(pass->getPassImplementation());
		if (pass->getPassImplementation() != NULL && rosePass == NULL)
			throw std::string("Oh my god, what is going on");
		return rosePass;
	}
	SgProject* project;

	std::vector<InstRO::Pass*> passBucket;
	std::vector<InstRO::Core::PassImplementation*> passImplementationBucket;

 public:
	class GenericAdapterConfiguration {
	 public:
		GenericAdapterConfiguration() { loopPass = functionPass = loopBodyPass = NULL; }
		void instrumentFunctions(Pass* functionSelector) { functionPass = functionSelector; };
		void instrumentLoopConstruct(Pass* loopConstructSelector) { loopPass = loopConstructSelector; };
		void instrumentLoopBody(Pass* loopBodySelector) { loopBodyPass = loopBodySelector; };
		Pass* getFunctionSelector() { return functionPass; }
		Pass* getLoopConstructSelector() { return loopPass; }
		Pass* getLoopBodySelector() { return loopBodyPass; }

	 protected:
		Pass* loopPass, *functionPass, *loopBodyPass;
	};

 public:
	 virtual InstRO::Pass* createMatthiasZoellnerLoopInstrumentationAdapter(InstRO::Pass * pass);
	
	virtual InstRO::Pass* createConstructHierarchyASTDotGenerator(InstRO::Pass* pass, std::string fileName);
	virtual InstRO::Pass* createProgramEntrySelector();
	virtual InstRO::Pass* createFunctionSelector();
	// Text Based Selection in Various Flavors
	virtual InstRO::Pass* createIdentifyerSelector(std::vector<std::string> matchList);	// * Match Identifyers against the matchList
	virtual InstRO::Pass* createIdentifyerFilter(std::vector<std::string> matchList, Pass* filterInput);

	virtual InstRO::Pass* createFunctionBlackAndWhiteListSelector(std::vector<std::string> rules);
	virtual InstRO::Pass* createFunctionBlackAndWhiteListFilter(std::vector<std::string> rules, Pass* inputPasses);
	//	virtual InstRO::Pass* createFunctionBlackNWhiteSelector(std::string string);

	virtual InstRO::Pass* createBooleanAndSelector(InstRO::Pass* inputA, InstRO::Pass* inputB);
	virtual InstRO::Pass* createBooleanOrSelector(InstRO::Pass* inputA, InstRO::Pass* inputB);
	// Call Path Based selection
	virtual InstRO::Pass* createCallPathSelector(InstRO::Pass* callees, InstRO::Pass* caller);

	InstRO::Pass* createConstructClassSelector(InstRO::Core::ConstructTraitType constructClass);
	// Elevator Selectors
	virtual InstRO::Pass* createConstructRaisingElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType level);
	virtual InstRO::Pass* createConstructLoweringElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType level);
	virtual InstRO::Pass* createConstructCroppingElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType minLevel, InstRO::Core::ConstructTraitType maxLevel);
	// OpenMP Stuff
	virtual InstRO::Pass* createOpenMPSelector();
	virtual InstRO::Pass* createOpenMPFilter(Pass* input);
	virtual InstRO::Pass* createOpenMPOpariCannonizer(Pass* input);
	virtual InstRO::Pass* createOPARIAdapter(Pass* input);

	// UniqueCallpathTransformer
	virtual InstRO::Pass* createUniqueCallpathTransformer(Pass* input);
	virtual InstRO::Pass* createUniqueCallpathTransformer(Pass* input, Pass* root, Pass* active);
	// Adapter
	virtual InstRO::Pass* createGPIAdapter(InstRO::Pass* input);
	virtual InstRO::Pass* createConstructPrinter(InstRO::Pass* pass);
	virtual InstRO::Pass* createGenericAdapter(GenericAdapterConfiguration gac);
	virtual InstRO::Pass* createGenericAdapter(Pass* functionSelection, Pass* loopSelection, Pass* branchingSelection);
};
}
}
#endif
