
#include "instro.h"
#include <iostream>

#ifdef USE_ROSE
#include "lib/RoseTestSupport.h"
#endif

/**
 * This is the TestInstrumentor implementation.
 *
 * It expects a file with the expected items, one per line, exported to the environment variable
 * INSTRO_TEST_INPUT_FILENAME. The check is not performed on the fully qualified filename (as returned in the
 * Construct's identifier) but only the filename. Fully qualified path is strongly preferred.
 *
 * TODO Have the selection graph runtime configurable (JSON?)
 *
 */

// reads environment variable to return the filename
std::string getInputFilename() {
	std::string filename;
	char *ev = getenv("INSTRO_TEST_INPUT_FILENAME");
	if (ev != nullptr) {
		filename = std::string(ev);
	} else {
		exit(-2);
	}

	return filename;
}

/**
 * Actual instrumentor
 */
int main(int argc, char **argv) {
/*
 * We want to use the same binary for both Rose and Clang
 */
#ifdef USE_ROSE
	using InstrumentorType = RoseTest::RoseTestInstrumentor;
#endif

	InstrumentorType instrumentor(argc, argv);
	auto factory = instrumentor.getFactory();

	std::string filename = getInputFilename();

	auto ctFuncLvlSelector = factory->createConstructClassSelector(InstRO::Core::ConstructTraitType::CTFunction);
	auto ctLoopLvlSelector = factory->createConstructClassSelector(InstRO::Core::ConstructTraitType::CTLoopStatement);
	auto ctStmtLvlSelector = factory->createConstructClassSelector(InstRO::Core::ConstructTraitType::CTStatement);
	auto ctCondLvlSelector =
			factory->createConstructClassSelector(InstRO::Core::ConstructTraitType::CTConditionalStatement);

	// sink, so we ignore the returned Pass *
	factory->createTestAdapter(ctFuncLvlSelector, "CTFunctionSelector", filename);
	factory->createTestAdapter(ctLoopLvlSelector, "CTLoopSelector", filename);
	factory->createTestAdapter(ctStmtLvlSelector, "CTStatementSelector", filename);
	factory->createTestAdapter(ctCondLvlSelector, "CTConditionalSelector", filename);

	instrumentor.apply();

	// Returns false if everything was fine, true otherwise
	return instrumentor.testFailed();
}
