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

// Test for the RoseFunctionWrapper transformer that examines the constructs selected by the transformer and their
// expressions. For each test input file X.in a xml configuration file X.conf is expected to be available in the same
// directory. This file is used to specify the operation mode of the transformer, e.g., the name transformer, definition
// prefix, wrapper prefix, and the identifiers servering as input for the transformer and the optional renaming
// selector.

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

std::vector<std::string> getIdentifiers(const pt::ptree& testConfig, const std::string& key) {
	std::vector<std::string> identifiers;
	if (testConfig.get_child_optional(key)) {
		for (const pt::ptree::value_type& v : testConfig.get_child(key)) {
			identifiers.push_back(v.second.data());
		}
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

	auto inputSelector = factory->createIdentifierMatcherSelector(getIdentifiers(testConfig, "test.input"));
	auto renamingIdentifiers = getIdentifiers(testConfig, "test.renaming");
	InstRO::Pass* renamingSelector =
			renamingIdentifiers.empty() ? nullptr : factory->createIdentifierMatcherSelector(renamingIdentifiers);
	auto functionWrapper = factory->createRoseFunctionWrapper(
			inputSelector, renamingSelector, getNameTransformer(testConfig), definitionPrefix, wrapperPrefix);
	auto functionWrapperExpressions =
			factory->createConstructLoweringElevator(functionWrapper, InstRO::Core::ConstructTraitType::CTExpression);

#if 0
	// debug output
	factory->createConstructPrinterAdapter(functionWrapper);
	factory->createConstructPrinterAdapter(functionWrapperExpressions);
#endif

	factory->createTestAdapter(functionWrapper, "FunctionWrapperTransformations", filename);
	factory->createTestAdapter(functionWrapperExpressions, "FunctionWrapperTransformationExpressions", filename);

	instrumentor.apply();

	// Returns false if everything was fine, true otherwise
	return instrumentor.testFailed();
}
