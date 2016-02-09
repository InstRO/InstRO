
#include "instro.h"
#include <iostream>

#ifdef INSTRO_USE_ROSE
#include "lib/RoseTestSupport.h"
#endif

#include "instro/utility/Logger.h"
#include "instro/utility/Environment.h"

int main(int argc, char **argv) {
/*
 * We want to use the same binary for both Rose and Clang
 */
#ifdef INSTRO_USE_ROSE
	using InstrumentorType = RoseTest::RoseTestInstrumentor;
	using CTrait = InstRO::Core::ConstructTraitType;

	InstrumentorType instrumentor(argc, argv);
	auto factory = instrumentor.getFactory();

	std::string filename = InstRO::Utility::getEnvironmentVariable("INSTRO_TEST_INPUT_FILENAME");

	auto selA = factory->createConstructTraitSelector(CTrait::CTFunction);
	auto selB = factory->createConstructTraitSelector(CTrait::CTStatement);

	auto andSelector = factory->createBooleanAndSelector(selA, selB);
	auto orSelector = factory->createBooleanOrSelector(selA, selB);

	factory->createTestAdapter(andSelector, "FunctionAndStatementSelector", filename);
	factory->createTestAdapter(orSelector, "FunctionOrStatementSelector", filename);

	instrumentor.apply();

	return instrumentor.testFailed();
#endif

#ifdef USE_CLANG
	logIt(ERROR) << "Not implemented yet!" << std::endl;
	return -1;
#endif
}
