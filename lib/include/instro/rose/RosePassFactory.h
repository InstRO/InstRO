#ifndef INSTRO_ROSE_PASSFACTORY_H
#define INSTRO_ROSE_PASSFACTORY_H

#include "instro/core/Pass.h"
#include "instro/core/PassFactory.h"
#include "instro/core/PassManager.h"
#include "instro/rose/core/RosePassImplementation.h"

#include "instro/rose/pass/transformer/FunctionWrapper.h"

#include <rose.h>

namespace InstRO {
namespace Rose {

class RosePassFactory : public InstRO::PassFactory {
 public:
	RosePassFactory(PassManagement::PassManager* refManager, SgProject* proj) : PassFactory(refManager), project(proj) {}

 private:
	SgProject* project;

 public:
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
