
#include "instro.h"
#include <iostream>

#ifdef USE_ROSE
#include "lib/RoseTestSupport.h"
#endif

#include "instro/utility/Logger.h"
#include "instro/utility/Environment.h"

/**
 * Implicitly tests the Extended Call Graph as this is used as basis within the Callpath-Selector
 */
int main(int argc, char **argv) {
/*
 * We want to use the same binary for both Rose and Clang
 */
#ifdef USE_ROSE
	using InstrumentorType = RoseTest::RoseTestInstrumentor;
	using CTrait = InstRO::Core::ConstructTraitType;

	InstrumentorType instrumentor(argc, argv);
	auto factory = instrumentor.getFactory();

	std::string filename = InstRO::Utility::getEnvironmentVariable("INSTRO_TEST_INPUT_FILENAME");

	auto programEntrySelector = factory->createProgramEntrySelector();
	auto functionSelector = factory->createConstructClassSelector(CTrait::CTFunction);
	auto cpSelector = factory->createCallpathSelector(programEntrySelector, functionSelector);

	factory->createTestAdapter(programEntrySelector, "EntrySelector", filename);
	factory->createTestAdapter(cpSelector, "CallpathSelector", filename);

#ifdef CDEBUG
#endif

	instrumentor.apply();

	return instrumentor.testFailed();
#endif

#ifdef USE_CLANG
	logIt(ERROR) << "Not implemented yet!" << std::endl;
	return 0;
#endif
}
