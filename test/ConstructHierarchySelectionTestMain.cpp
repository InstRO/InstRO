
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
 * Construct's identifier) but only the filename. 
 * Fully qualified path to the input file is strongly preferred.
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
 *
 * This instrumentor uses the ConstructClassSelector to select each of
 * - expressions
 * - scope statements
 * - conditional statements
 * - loop statements
 * - statements
 * - functions
 * each of these selectors is fed into one TestAdapter instance. The TestAdapter reads the given input file and outputs
 * the result to the TestSummary object.
 * If all selectors produce the expected ConstructSet the app returns 0.
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
	auto ctScopeLvlSelector = factory->createConstructClassSelector(InstRO::Core::ConstructTraitType::CTScopeStatement);
	auto ctExprLvlSelector = factory->createConstructClassSelector(InstRO::Core::ConstructTraitType::CTExpression);

	// sink, so we ignore the returned Pass *
	factory->createTestAdapter(ctFuncLvlSelector, "CTFunctionSelector", filename);
	factory->createTestAdapter(ctLoopLvlSelector, "CTLoopSelector", filename);
	factory->createTestAdapter(ctStmtLvlSelector, "CTStatementSelector", filename);
	factory->createTestAdapter(ctCondLvlSelector, "CTConditionalSelector", filename);
	factory->createTestAdapter(ctScopeLvlSelector, "CTScopeSelector", filename);
	factory->createTestAdapter(ctExprLvlSelector, "CTExpressionSelector", filename);

	instrumentor.apply();

	// Returns false if everything was fine, true otherwise
	return instrumentor.testFailed();
}
