#include "instro/rose/RosePassFactory.h"

#include "instro/rose/pass/adapter/RoseStrategyBasedAdapter.h"
#include "instro/rose/pass/adapter/RoseDefaultInstrumentationAdapter.h"
#include "instro/rose/pass/transformer/RoseUniqueCallpathTransformer.h"

#include <string>
#include <vector>

namespace InstRO {
namespace Rose {

Pass* RosePassFactory::createDefaultInstrumentationAdapter(InstRO::Pass* input) {
	Pass* newPass = new Pass(new InstRO::Rose::Adapter::RoseDefaultInstrumentationAdapter(project),
													 InstRO::Core::ChannelConfiguration(input));
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
															 std::move(my_strategy), std::move(my_strategy2), std::move(my_strategy3)),
													 InstRO::Core::ChannelConfiguration(pass));
	newPass->setPassName("InstRO::Rose::Adapter::MatthiasZoellnerLoopInstrumentationAdapter");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseUniqueCallpathTransformer(InstRO::Pass* input) {
	InstRO::Core::ChannelConfiguration cfg(input);
	cfg.setConstructLevel(input, InstRO::Core::ConstructTraitType::CTFunction,
												InstRO::Core::ConstructTraitType::CTFunction);

	InstRO::Pass* newPass = new InstRO::Pass(
			new Transformer::RoseUniqueCallpathTransformer(Transformer::RoseUniqueCallpathTransformer::Mode::nMode), cfg);
	newPass->setPassName("InstRO::Rose::Transformer::RoseUniqueCallpathTransformer");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseUniqueCallpathTransformer(Pass* input, Pass* root, Pass* active) {
	Transformer::RoseUniqueCallpathTransformer::Mode m = Transformer::RoseUniqueCallpathTransformer::Mode::nMode;
	if (root == nullptr) {
		if (active != nullptr) {
			m = Transformer::RoseUniqueCallpathTransformer::Mode::aMode;
		}
	} else {
		if (active == nullptr) {
			m = Transformer::RoseUniqueCallpathTransformer::Mode::rMode;
		} else {
			m = Transformer::RoseUniqueCallpathTransformer::Mode::raMode;
		}
	}
	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::RoseUniqueCallpathTransformer(m),
																					 InstRO::Core::ChannelConfiguration(input, root, active));
	newPass->setPassName("InstRO::Rose::Transformer::RoseUniqueCallpathTransformer");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseFunctionWrapper(
		InstRO::Pass* input, InstRO::Rose::Transformer::RoseFunctionWrapper::NameTransformer nameTransformer) {
	InstRO::Core::ChannelConfiguration cfg(input);
	cfg.setConstructLevel(input, InstRO::Core::ConstructTraitType::CTExpression,
												InstRO::Core::ConstructTraitType::CTFunction);

	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::RoseFunctionWrapper(nameTransformer), cfg);
	newPass->setPassName("InstRO::Rose::Transformer::RoseFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}
InstRO::Pass* RosePassFactory::createRoseFunctionWrapper(
		InstRO::Pass* input, InstRO::Pass* renaming,
		InstRO::Rose::Transformer::RoseFunctionWrapper::NameTransformer nameTransformer,
		const std::string& definitionPrefix, const std::string& wrapperPrefix) {
	InstRO::Core::ChannelConfiguration cfg(input, renaming);
	cfg.setConstructLevel(input, InstRO::Core::ConstructTraitType::CTExpression,
												InstRO::Core::ConstructTraitType::CTFunction);
	cfg.setConstructLevel(renaming, InstRO::Core::ConstructTraitType::CTExpression,
												InstRO::Core::ConstructTraitType::CTGlobalScope);
	InstRO::Pass* newPass =
			new InstRO::Pass(new Transformer::RoseFunctionWrapper(nameTransformer, definitionPrefix, wrapperPrefix), cfg);
	newPass->setPassName("InstRO::Rose::Transformer::RoseFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseMPIFunctionWrapper(InstRO::Pass* input) {
	InstRO::Pass* newPass =
			new InstRO::Pass(new Transformer::RoseMPIFunctionWrapper(), InstRO::Core::ChannelConfiguration(input));
	newPass->setPassName("InstRO::Rose::Transformer::RoseMPIFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseMPIFunctionWrapper(InstRO::Pass* input, InstRO::Pass* renaming,
																														const std::string& definitionPrefix,
																														const std::string& wrapperPrefix) {
	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::RoseMPIFunctionWrapper(definitionPrefix, wrapperPrefix),
																					 InstRO::Core::ChannelConfiguration(input, renaming));
	newPass->setPassName("InstRO::Rose::Transformer::RoseMPIFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

}	// Rose
}	// InstRO
