
#include "instro.h"
#include <iostream>

#if INSTRO_USE_ROSE
#include "lib/RoseTestSupport.h"
#endif

#include "instro/utility/Logger.h"
#include "instro/utility/Environment.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/split.hpp>
#include <sstream>

/**
 * This is the TestInstrumentor implementation.
 *
 * It expects a file with the expected items, one per line, exported to the environment variable
 * INSTRO_TEST_INPUT_FILENAME. The check is not performed on the fully qualified filename (as returned in the
 * Construct's identifier) but only the filename.
 * Fully qualified path to the input file is strongly preferred.
 *
 */

/**
* This test verifies the output of the UniqueCallpathTransformer. The transformer selects all newly created functions
* and modified function calls.
*/

std::vector<std::string> getActiveIdentifiers(const std::string &inFileName) {
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

int main(int argc, char **argv) {
#if INSTRO_USE_ROSE
	using InstrumentorType = RoseTest::RoseTestInstrumentor;
	InstrumentorType instrumentor(argc, argv);
#endif

	auto factory = instrumentor.getFactory();

	std::string filename = InstRO::Utility::getEnvironmentVariable("INSTRO_TEST_INPUT_FILENAME");

	auto identifierSelector = factory->createIdentifierMatcherSelector(std::vector<std::string>{"::markMe"});

	auto activeIdentifiers = getActiveIdentifiers(filename);
#if INSTRO_USE_ROSE
	InstRO::Pass *ucpTransformer;
	if (activeIdentifiers.empty()) {
		ucpTransformer = factory->createRoseUniqueCallpathTransformer(identifierSelector);
	} else {
		auto functionSelector = factory->createConstructTraitSelector(InstRO::Core::ConstructTraitType::CTFunction);
		auto activeIdentifierSelector = factory->createIdentifierMatcherSelector(activeIdentifiers);
		auto activeSelector = factory->createBooleanAndSelector(activeIdentifierSelector, functionSelector);
		ucpTransformer = factory->createRoseUniqueCallpathTransformer(identifierSelector, nullptr, activeSelector);
	}
#endif

#if 0
	factory->createConstructPrinterAdapter(ucpTransformer);
#endif

	factory->createTestAdapter(ucpTransformer, "UCPTransformations", filename);

	instrumentor.apply();

	// Returns false if everything was fine, true otherwise
	return instrumentor.testFailed();
}
