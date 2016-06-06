#include "instro/rose/RosePassFactory.h"

#include "instro/rose/pass/adapter/RoseStrategyBasedAdapter.h"
#include "instro/rose/pass/adapter/RoseDefaultInstrumentationAdapter.h"
#include "instro/rose/pass/adapter/RoseScorepRegionInstrumentationAdapter.h"
#include "instro/rose/pass/transformer/RoseUniqueCallpathTransformer.h"

#include "instro/utility/Environment.h"

#include <string>
#include <vector>

namespace InstRO {
namespace Rose {

Pass* RosePassFactory::createDefaultInstrumentationAdapter(InstRO::Pass* input) {
	Pass* newPass =
			new Pass(new InstRO::Rose::Adapter::RoseDefaultInstrumentationAdapter(project),
							 InstRO::Core::ChannelConfiguration(input), "InstRO::Rose::Adapter::RoseDefaultInstrumentationAdapter");
	passManager->registerPass(newPass);
	return newPass;
}

/* ROSE ONLY */

InstRO::Pass* RosePassFactory::createScorepRegionInstrumentationAdapter(InstRO::Pass* input) {
	Pass* newPass = new Pass(new InstRO::Rose::Adapter::RoseScorepRegionInstrumentationAdapter(project),
													 InstRO::Core::ChannelConfiguration(input),
													 "InstRO::Rose::Adapter::RoseScorepRegionInstrumentationAdapter");

	// we need to adjust the commandline...
	auto ret = InstRO::Utility::getScorepIncludeFlags();

	auto pos = ret.find(" ");
	auto first = ret.substr(0, pos);
	auto second = ret.substr(pos + 1, ret.size() - (pos + 2));

	auto sgStrList = project->get_originalCommandLineArgumentList();

	std::vector<std::string> myArgv;
	myArgv.push_back(sgStrList[0]);
	myArgv.push_back(first);
	myArgv.push_back(second);

	for (int i = 1; i < sgStrList.size(); ++i) {
		myArgv.push_back(sgStrList[i]);
	}

	project->processCommandLine(myArgv);
	project->parse();

	passManager->registerPass(newPass);
	return newPass;
}

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
													 InstRO::Core::ChannelConfiguration(pass),
													 "InstRO::Rose::Adapter::MatthiasZoellnerLoopInstrumentationAdapter");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseUniqueCallpathTransformer(InstRO::Pass* input) {
	InstRO::Core::ChannelConfiguration cfg(input);
	cfg.setConstructLevel(input, InstRO::Core::ConstructTraitType::CTFunction,
												InstRO::Core::ConstructTraitType::CTFunction);

	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::RoseUniqueCallpathTransformer(), cfg,
																					 "InstRO::Rose::Transformer::RoseUniqueCallpathTransformer");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseUniqueCallpathTransformer(Pass* input, Pass* root, Pass* active) {
	using ChannelConfiguration = InstRO::Core::ChannelConfiguration;
	using ConfigTuple = InstRO::Core::ChannelConfiguration::ConfigTuple;
	using PassType = InstRO::Rose::Transformer::RoseUniqueCallpathTransformer::PassType;

	Transformer::RoseUniqueCallpathTransformer::InputMapping mapping{std::make_pair(PassType::InputPT, 0)};
	ChannelConfiguration channelConfig;

	if (root) {
		mapping.insert(std::make_pair(PassType::RootPT, 1));
		if (active) {
			mapping.insert(std::make_pair(PassType::ActivePT, 2));
			channelConfig = ChannelConfiguration(ConfigTuple(0, input), ConfigTuple(1, root), ConfigTuple(2, active));
		} else {
			channelConfig = ChannelConfiguration(ConfigTuple(0, input), ConfigTuple(1, root));
		}
	} else {
		if (active) {
			mapping.insert(std::make_pair(PassType::ActivePT, 1));
			channelConfig = ChannelConfiguration(ConfigTuple(0, input), ConfigTuple(1, active));
		} else {
			channelConfig = ChannelConfiguration(input);
		}
	}

	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::RoseUniqueCallpathTransformer(mapping), channelConfig,
																					 "InstRO::Rose::Transformer::RoseUniqueCallpathTransformer");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseFunctionWrapper(
		InstRO::Pass* input, InstRO::Rose::Transformer::RoseFunctionWrapper::NameTransformer nameTransformer) {
	InstRO::Core::ChannelConfiguration cfg(input);
	cfg.setConstructLevel(input, InstRO::Core::ConstructTraitType::CTExpression,
												InstRO::Core::ConstructTraitType::CTFunction);

	InstRO::Pass* newPass = new InstRO::Pass(new Transformer::RoseFunctionWrapper(nameTransformer), cfg,
																					 "InstRO::Rose::Transformer::RoseFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}
InstRO::Pass* RosePassFactory::createRoseFunctionWrapper(
		InstRO::Pass* input, InstRO::Pass* renaming,
		InstRO::Rose::Transformer::RoseFunctionWrapper::NameTransformer nameTransformer,
		const std::string& definitionPrefix, const std::string& wrapperPrefix) {
	using ChannelConfiguration = InstRO::Core::ChannelConfiguration;
	using ConfigTuple = InstRO::Core::ChannelConfiguration::ConfigTuple;

	bool useRenamingPass = renaming != nullptr;
	ChannelConfiguration cfg;
	if (useRenamingPass) {
		cfg = ChannelConfiguration(ConfigTuple(0, input), ConfigTuple(1, renaming));
		cfg.setConstructLevel(renaming, InstRO::Core::ConstructTraitType::CTExpression,
													InstRO::Core::ConstructTraitType::CTGlobalScope);
	} else {
		cfg = ChannelConfiguration(input);
	}
	cfg.setConstructLevel(input, InstRO::Core::ConstructTraitType::CTExpression,
												InstRO::Core::ConstructTraitType::CTFunction);

	InstRO::Pass* newPass = new InstRO::Pass(
			new Transformer::RoseFunctionWrapper(nameTransformer, definitionPrefix, wrapperPrefix, useRenamingPass), cfg,
			"InstRO::Rose::Transformer::RoseFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseMPIFunctionWrapper(InstRO::Pass* input) {
	InstRO::Pass* newPass =
			new InstRO::Pass(new Transformer::RoseMPIFunctionWrapper(), InstRO::Core::ChannelConfiguration(input),
											 "InstRO::Rose::Transformer::RoseMPIFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

InstRO::Pass* RosePassFactory::createRoseMPIFunctionWrapper(InstRO::Pass* input, InstRO::Pass* renaming,
																														const std::string& definitionPrefix,
																														const std::string& wrapperPrefix) {
	using ChannelConfiguration = InstRO::Core::ChannelConfiguration;
	using ConfigTuple = InstRO::Core::ChannelConfiguration::ConfigTuple;

	bool useRenamingPass = renaming != nullptr;
	ChannelConfiguration cfg;
	if (useRenamingPass) {
		cfg = ChannelConfiguration(ConfigTuple(0, input), ConfigTuple(1, renaming));
		cfg.setConstructLevel(renaming, InstRO::Core::ConstructTraitType::CTExpression,
													InstRO::Core::ConstructTraitType::CTGlobalScope);
	} else {
		cfg = ChannelConfiguration(input);
	}
	cfg.setConstructLevel(input, InstRO::Core::ConstructTraitType::CTExpression,
												InstRO::Core::ConstructTraitType::CTFunction);

	InstRO::Pass* newPass =
			new InstRO::Pass(new Transformer::RoseMPIFunctionWrapper(definitionPrefix, wrapperPrefix, useRenamingPass), cfg,
											 "InstRO::Rose::Transformer::RoseMPIFunctionWrapper");
	passManager->registerPass(newPass);
	return newPass;
}

}	// Rose
}	// InstRO
