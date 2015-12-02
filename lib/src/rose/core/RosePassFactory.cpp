#include "instro/rose/RosePassFactory.h"

#include "instro/rose/pass/transformer/UniqueCallpathTransformer.h"
#include "instro/rose/pass/adapter/RoseStrategyBasedAdapter.h"
#include "instro/rose/pass/adapter/RoseDefaultInstrumentationAdapter.h"

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

InstRO::Pass* RosePassFactory::createMatthiasZoellnerLoopInstrumentationAdapter(InstRO::Pass* pass) {
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

InstRO::Pass* RosePassFactory::createUniqueCallpathTransformer(InstRO::Pass* input) {
	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::UniqueCallpathTransformer(input));
	newPass->setPassName("InstRO::Rose::Transformer::UniqueCallpathTransformer");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createUniqueCallpathTransformer(Pass* input, Pass* root, Pass* active) {
	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::UniqueCallpathTransformer(input, root, active));
	newPass->setPassName("InstRO::Rose::Transformer::UniqueCallpathTransformer");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createFunctionWrapper(
		InstRO::Pass* input, InstRO::Rose::Transformer::FunctionWrapper::NameTransformer nameTransformer) {
	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::FunctionWrapper(input, nameTransformer));
	newPass->setPassName("InstRO::Rose::Transformer::FunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}
InstRO::Pass* RosePassFactory::createFunctionWrapper(
		InstRO::Pass* input, InstRO::Pass* renaming,
		InstRO::Rose::Transformer::FunctionWrapper::NameTransformer nameTransformer, const std::string& definitionPrefix,
		const std::string& wrapperPrefix) {
	InstRO::Pass* newPass = new InstRO::Pass(
			new Transformer::FunctionWrapper(input, renaming, nameTransformer, definitionPrefix, wrapperPrefix));
	newPass->setPassName("InstRO::Rose::Transformer::FunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createMPIFunctionWrapper(InstRO::Pass* input) {
	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::MPIFunctionWrapper(input));
	newPass->setPassName("InstRO::Rose::Transformer::MPIFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createMPIFunctionWrapper(InstRO::Pass* input, InstRO::Pass* renaming,
																												const std::string& definitionPrefix,
																												const std::string& wrapperPrefix) {
	InstRO::Pass* newPass =
			new InstRO::Pass(new Transformer::MPIFunctionWrapper(input, renaming, definitionPrefix, wrapperPrefix));
	newPass->setPassName("InstRO::Rose::Transformer::MPIFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

}	// Rose
}	// InstRO
