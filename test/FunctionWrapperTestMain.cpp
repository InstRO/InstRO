#include "instro.h"

#include "lib/RoseTestSupport.h"
#include "instro/rose/pass/transformer/RoseFunctionWrapper.h"

#include "instro/utility/Logger.h"
#include "instro/utility/Environment.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace RoseTransformer = InstRO::Rose::Transformer;
namespace pt = boost::property_tree;

pt::ptree getConfig(const std::string& inFilename) {
	boost::filesystem::path configFile = inFilename;
	configFile.replace_extension("conf");

	pt::ptree tree;
	pt::read_xml(configFile.string(), tree);
	return tree;
}

RoseTransformer::RoseFunctionWrapper::NameTransformer getNameTransformer(const pt::ptree& testConfig) {
	const std::string transformerId = testConfig.get("test.nameTransformer", "identity");
	if (transformerId == "identity") {
		return RoseTransformer::RoseFunctionWrapper::IdentityNameTransformer();
	} else if (transformerId == "mpi") {
		return RoseTransformer::RoseMPIFunctionWrapper::PMPINameTransformer();
	} else {
		InstRO::logIt(InstRO::WARN) << "Unknown name transformer (" << transformerId
																<< "), falling back to identity name transformer" << std::endl;
		return RoseTransformer::RoseFunctionWrapper::IdentityNameTransformer();
	}
}

std::vector<std::string> getInputIdentifiers(const pt::ptree& testConfig) {
	std::vector<std::string> identifiers;
	for (const pt::ptree::value_type& v : testConfig.get_child("test.input")) {
		identifiers.push_back(v.second.data());
	}
	return identifiers;
}

int main(int argc, char** argv) {
	using InstrumentorType = RoseTest::RoseTestInstrumentor;
	InstrumentorType instrumentor(argc, argv);

	auto factory = instrumentor.getFactory();

	std::string filename = InstRO::Utility::getEnvironmentVariable("INSTRO_TEST_INPUT_FILENAME");
	pt::ptree testConfig = getConfig(filename);

	const std::string definitionPrefix = testConfig.get("test.definitionPrefix", std::string());
	const std::string wrapperPrefix = testConfig.get("test.wrapperPrefix", std::string());

	auto inputSelector = factory->createIdentifierMatcherSelector(getInputIdentifiers(testConfig));
	auto functionWrapper = factory->createRoseFunctionWrapper(inputSelector, nullptr, getNameTransformer(testConfig),
																														definitionPrefix, wrapperPrefix);
	auto functionWrapperExpressions =
			factory->createConstructLoweringElevator(functionWrapper, InstRO::Core::ConstructTraitType::CTExpression);

#if 1
	factory->createConstructPrinterAdapter(functionWrapper);
	factory->createConstructPrinterAdapter(functionWrapperExpressions);
#endif

	factory->createTestAdapter(functionWrapper, "FunctionWrapperTransformations", filename);
	factory->createTestAdapter(functionWrapperExpressions, "FunctionWrapperTransformationExpressions", filename);

	instrumentor.apply();

	// Returns false if everything was fine, true otherwise
	return instrumentor.testFailed();
}
