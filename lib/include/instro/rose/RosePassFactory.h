#ifndef INSTRO_ROSE_ROSEPASSFACTORY_H
#define INSTRO_ROSE_ROSEPASSFACTORY_H

#include "instro/Pass.h"
#include "instro/PassFactory.h"
#include "instro/core/PassManager.h"
#include "instro/rose/core/RosePassImplementation.h"
#include "instro/rose/pass/transformer/RoseFunctionWrapper.h"

#include <rose.h>

namespace InstRO {
namespace Rose {

class RosePassFactory : public InstRO::PassFactory {
 public:
	RosePassFactory(PassManagement::PassManager* refManager, SgProject* proj) : PassFactory(refManager), project(proj) {}

 private:
	SgProject* project;

 public:
	Pass* createDefaultInstrumentationAdapter(InstRO::Pass* input);

	/* ROSE ONLY */

	InstRO::Pass* createScorepRegionInstrumentationAdapter(InstRO::Pass *input);

	// RoseUniqueCallpathTransformer
	InstRO::Pass* createRoseUniqueCallpathTransformer(Pass* input);
	InstRO::Pass* createRoseUniqueCallpathTransformer(Pass* input, Pass* root, Pass* active);

	// RoseFunctionWrapper
	InstRO::Pass* createRoseFunctionWrapper(
			InstRO::Pass* input, InstRO::Rose::Transformer::RoseFunctionWrapper::NameTransformer nameTransformer);
	InstRO::Pass* createRoseFunctionWrapper(
			InstRO::Pass* input, InstRO::Pass* renaming,
			InstRO::Rose::Transformer::RoseFunctionWrapper::NameTransformer nameTransformer,
			const std::string& definitionPrefix, const std::string& wrapperPrefix);
	InstRO::Pass* createRoseMPIFunctionWrapper(InstRO::Pass* input);
	InstRO::Pass* createRoseMPIFunctionWrapper(InstRO::Pass* input, InstRO::Pass* renaming,
																						 const std::string& definitionPrefix, const std::string& wrapperPrefix);

	InstRO::Pass* createRoseMatthiasZoellnerLoopInstrumentationAdapter(InstRO::Pass* pass);
};
}	// namespace Rose
}	// namespace InstRO

#endif
