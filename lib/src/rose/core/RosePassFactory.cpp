#include "instro/rose/RosePassFactory.h"

#include "instro/rose/pass/adapter/RoseStrategyBasedAdapter.h"
#include "instro/rose/pass/adapter/RoseDefaultInstrumentationAdapter.h"
#include "instro/rose/pass/transformer/RoseUniqueCallpathTransformer.h"

#include <string>
#include <vector>

namespace InstRO {
namespace Rose {

Pass* RosePassFactory::createDefaultInstrumentationAdapter(InstRO::Pass* input) {
	Pass* newPass = new Pass(new InstRO::Rose::Adapter::RoseDefaultInstrumentationAdapter(input, project));
	newPass->setPassName("InstRO::Rose::Adapter::RoseDefaultInstrumentationAdapter");
	passManager->registerPass(newPass);
	return newPass;
}

/* ROSE ONLY */

InstRO::Pass* RosePassFactory::createRoseMatthiasZoellnerLoopInstrumentationAdapter(InstRO::Pass* pass) {
	auto initializer = std::make_shared<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::ScorePInitializer>();
	std::unique_ptr<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy> my_strategy =
			std::make_unique<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::ScorePStatementWrapperStrategy>(initializer);
	std::unique_ptr<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy> my_strategy2 =
			std::make_unique<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::ScorePLoopIterationStrategy>(initializer);
	std::unique_ptr<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::GenericInstrumentationStrategy> my_strategy3 =
			std::make_unique<InstRO::Rose::Adapter::StrategyBasedAdapterSupport::ScorePFunctionScopeStrategy>(initializer);

	Pass* newPass = new Pass(new InstRO::Rose::Adapter::RoseStrategyBasedAdapter(
			pass, std::move(my_strategy), std::move(my_strategy2), std::move(my_strategy3)));
	newPass->setPassName("InstRO::Rose::Adapter::MatthiasZoellnerLoopInstrumentationAdapter");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseUniqueCallpathTransformer(InstRO::Pass* input) {
	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::RoseUniqueCallpathTransformer(input));
	newPass->setPassName("InstRO::Rose::Transformer::RoseUniqueCallpathTransformer");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseUniqueCallpathTransformer(Pass* input, Pass* root, Pass* active) {
	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::RoseUniqueCallpathTransformer(input, root, active));
	newPass->setPassName("InstRO::Rose::Transformer::RoseUniqueCallpathTransformer");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseFunctionWrapper(
		InstRO::Pass* input, InstRO::Rose::Transformer::RoseFunctionWrapper::NameTransformer nameTransformer) {
	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::RoseFunctionWrapper(input, nameTransformer));
	newPass->setPassName("InstRO::Rose::Transformer::RoseFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}
InstRO::Pass* RosePassFactory::createRoseFunctionWrapper(
		InstRO::Pass* input, InstRO::Pass* renaming,
		InstRO::Rose::Transformer::RoseFunctionWrapper::NameTransformer nameTransformer, const std::string& definitionPrefix,
		const std::string& wrapperPrefix) {
	InstRO::Pass* newPass = new InstRO::Pass(
			new Transformer::RoseFunctionWrapper(input, renaming, nameTransformer, definitionPrefix, wrapperPrefix));
	newPass->setPassName("InstRO::Rose::Transformer::RoseFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseMPIFunctionWrapper(InstRO::Pass* input) {
	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::RoseMPIFunctionWrapper(input));
	newPass->setPassName("InstRO::Rose::Transformer::RoseMPIFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseMPIFunctionWrapper(InstRO::Pass* input, InstRO::Pass* renaming,
																												const std::string& definitionPrefix,
																												const std::string& wrapperPrefix) {
	InstRO::Pass* newPass =
			new InstRO::Pass(new Transformer::RoseMPIFunctionWrapper(input, renaming, definitionPrefix, wrapperPrefix));
	newPass->setPassName("InstRO::Rose::Transformer::RoseMPIFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

}	// Rose
}	// InstRO
