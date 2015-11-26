
#include "instro.h"
#include <iostream>

#ifdef USE_ROSE
#include "lib/RoseTestSupport.h"
#endif

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

	auto idSelector = factory->createIdentifierMatcherSelector({"foo"});
	auto idSelector2 = factory->createIdentifierMatcherSelector({"f#"});

	factory->createTestAdapter(idSelector, "IdSelector", filename);
	factory->createTestAdapter(idSelector2, "IdSelector-f*", filename);

#ifdef DEBUG
	factory->createConstructPrinter(idSelector);
#endif

	instrumentor.apply();

	return instrumentor.testFailed();
#endif
}
