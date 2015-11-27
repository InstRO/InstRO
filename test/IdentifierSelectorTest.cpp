
#include "instro.h"
#include <iostream>

#ifdef USE_ROSE
#include "lib/RoseTestSupport.h"
#endif

#include "instro/utility/Logger.h"
#include "instro/utility/Environment.h"

int main(int argc, char **argv) {
/*
 * We want to use the same binary for both Rose and Clang
 */
#ifdef USE_ROSE
	using InstrumentorType = RoseTest::RoseTestInstrumentor;

	InstrumentorType instrumentor(argc, argv);
	auto factory = instrumentor.getFactory();

	std::string filename = InstRO::Utility::getEnvironmentVariable("INSTRO_TEST_INPUT_FILENAME");

	auto idSelector = factory->createIdentifierMatcherSelector({"::foo"});
	auto idSelector2 = factory->createIdentifierMatcherSelector({"::f#"});
	auto idSelector3 = factory->createIdentifierMatcherSelector({"#foo"});
	auto idSelector4 = factory->createIdentifierMatcherSelector({"#f#"});

	factory->createTestAdapter(idSelector, "IdSelector-::foo", filename);
	factory->createTestAdapter(idSelector2, "IdSelector-::f#", filename);
	factory->createTestAdapter(idSelector3, "IdSelector-#foo", filename);
	factory->createTestAdapter(idSelector4, "IdSelector-#f#", filename);

#ifdef DEBUG
	factory->createConstructPrinter(idSelector);
#endif

	instrumentor.apply();

	return instrumentor.testFailed();
#endif

#ifdef USE_CLANG
	logIt(ERROR) << "Not implemented yet!" << std::endl;
	return 0;
#endif
}
