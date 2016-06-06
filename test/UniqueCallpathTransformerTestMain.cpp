#include "instro.h"

#include "lib/RoseTestSupport.h"

#include "instro/utility/Logger.h"
#include "instro/utility/Environment.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <sstream>

// Test for the RoseUniqueCallpathTransformer that examines the constructs selected by the transformer (newly created
// functions and modified function calls). For each test input file X.in a separate X.active file, which contains a
// space delimited list of identifiers, may exist to specify the input for an optional active selector. Note that the
// test does not support specifying a root selector; adding support for this should be possible by reading the test
// configuration from a single file, just like the FunctionWrapperTest. Currently, the name of the input function, i.e.
// the one to create a unique call path for, is hardcoded to "::markMe".

std::vector<std::string> getActiveIdentifiers(const std::string& inFileName) {
	boost::filesystem::path activeFile = inFileName;
	activeFile.replace_extension("active");

	std::vector<std::string> actives;
	if (boost::filesystem::exists(activeFile)) {
		boost::filesystem::ifstream file(activeFile);
		std::stringstream buffer;
		buffer << file.rdbuf();

		std::string fileContent = buffer.str();
		boost::algorithm::trim(fileContent);
		boost::algorithm::split(actives, fileContent, boost::algorithm::is_space());
	}

	return actives;
}

int main(int argc, char** argv) {
	using InstrumentorType = RoseTest::RoseTestInstrumentor;
	InstrumentorType instrumentor(argc, argv);

	auto factory = instrumentor.getFactory();

	std::string filename = InstRO::Utility::getEnvironmentVariable("INSTRO_TEST_INPUT_FILENAME");

	auto identifierSelector = factory->createIdentifierMatcherSelector(std::vector<std::string>{"::markMe"});

	auto activeIdentifiers = getActiveIdentifiers(filename);

	InstRO::Pass* ucpTransformer;
	if (activeIdentifiers.empty()) {
		ucpTransformer = factory->createRoseUniqueCallpathTransformer(identifierSelector);
	} else {
		auto functionSelector = factory->createConstructTraitSelector(InstRO::Core::ConstructTraitType::CTFunction);
		auto activeIdentifierSelector = factory->createIdentifierMatcherSelector(activeIdentifiers);
		auto activeSelector = factory->createBooleanAndSelector(activeIdentifierSelector, functionSelector);
		ucpTransformer = factory->createRoseUniqueCallpathTransformer(identifierSelector, nullptr, activeSelector);
	}

#if 0
	// debug output
	factory->createConstructPrinterAdapter(ucpTransformer);
#endif

	factory->createTestAdapter(ucpTransformer, "UCPTransformations", filename);

	instrumentor.apply();

	// Returns false if everything was fine, true otherwise
	return instrumentor.testFailed();
}
