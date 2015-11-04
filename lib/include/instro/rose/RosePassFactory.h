#ifndef INSTRO_ROSE_PASSFACTORY_H
#define INSTRO_ROSE_PASSFACTORY_H

#include "instro/core/PassFactory.h"
#include "instro/core/PassManager.h"

#include "instro/rose/core/RosePassImplementation.h"
#include "instro/pass/selector/CompoundSelector.h"
#include "instro/rose/pass/adapter/RoseConstructPrinter.h"
#include "instro/rose/pass/adapter/ConstructHierarchyASTDotGenerator.h"
#include "instro/rose/pass/transformer/FunctionWrapper.h"

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
	InstRO::Pass* createBooleanAndSelector(InstRO::Pass* inputA, InstRO::Pass* inputB);
	InstRO::Pass* createBooleanOrSelector(InstRO::Pass* inputA, InstRO::Pass* inputB);

	InstRO::Pass* createIdentifierMatcherSelector(std::vector<std::string> matchList);
	InstRO::Pass* createCallpathSelector(InstRO::Pass* callees, InstRO::Pass* caller);
	InstRO::Pass* createConstructClassSelector(InstRO::Core::ConstructTraitType constructClass);
	InstRO::Pass* createAggregationStatementCountSelector(int threshold);

	// Elevator Selectors
	InstRO::Pass* createConstructRaisingElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType level);
	InstRO::Pass* createConstructLoweringElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType level);
	InstRO::Pass* createConstructCroppingElevator(InstRO::Pass* pass, InstRO::Core::ConstructTraitType minLevel,
																												InstRO::Core::ConstructTraitType maxLevel);

	InstRO::Pass* createInstROMeasurementInterfaceAdapter(InstRO::Pass* input);


	/* ROSE ONLY */

	// UniqueCallpathTransformer
	InstRO::Pass* createUniqueCallpathTransformer(Pass* input);
	InstRO::Pass* createUniqueCallpathTransformer(Pass* input, Pass* root, Pass* active);

	// FunctionWrapper
	InstRO::Pass* createFunctionWrapper(
			InstRO::Pass* input, InstRO::Rose::Transformer::FunctionWrapper::NameTransformer nameTransformer);
	InstRO::Pass* createFunctionWrapper(
			InstRO::Pass* input, InstRO::Pass* renaming,
			InstRO::Rose::Transformer::FunctionWrapper::NameTransformer nameTransformer, const std::string& definitionPrefix,
			const std::string& wrapperPrefix);
	InstRO::Pass* createMPIFunctionWrapper(InstRO::Pass* input);
	InstRO::Pass* createMPIFunctionWrapper(InstRO::Pass* input, InstRO::Pass* renaming,
																								 const std::string& definitionPrefix, const std::string& wrapperPrefix);


	InstRO::Pass* createConstructPrinter(InstRO::Pass* pass);

	InstRO::Pass* createMatthiasZoellnerLoopInstrumentationAdapter(InstRO::Pass* pass);

	InstRO::Pass* createConstructHierarchyASTDotGenerator(InstRO::Pass* pass, std::string fileName);
	InstRO::Pass* createProgramEntrySelector();

};

}
}
#endif
