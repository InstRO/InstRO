
#include "instro.h"
#include <iostream>

#if INSTRO_USE_ROSE
#include "lib/RoseTestSupport.h"
#endif

#include "instro/utility/Logger.h"
#include "instro/utility/Environment.h"

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
* This test verifies the output of the UniqueCallpathTransformer. The transformer selects all newly created functions and modified function calls.
*/

int main(int argc, char **argv) {

#if INSTRO_USE_ROSE
	using InstrumentorType = RoseTest::RoseTestInstrumentor;
	InstrumentorType instrumentor(argc, argv);
#endif

	auto factory = instrumentor.getFactory();

	std::string filename = InstRO::Utility::getEnvironmentVariable("INSTRO_TEST_INPUT_FILENAME");

	auto identifyerSelector = factory->createIdentifierMatcherSelector(std::vector<std::string> {"::markMe"});
#if INSTRO_USE_ROSE
	auto ucpTransformer = factory->createRoseUniqueCallpathTransformer(identifyerSelector);
#endif

	factory->createConstructPrinterAdapter(ucpTransformer);

	factory->createTestAdapter(ucpTransformer, "UCPTransformations", filename);

	instrumentor.apply();

	// Returns false if everything was fine, true otherwise
	return instrumentor.testFailed();
}
