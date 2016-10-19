// prevent any unused dependencies to boost_system
#define BOOST_SYSTEM_NO_DEPRECATED

#include "instro.h"

#if INSTRO_USE_ROSE
#include "lib/RoseTestSupport.h"
#endif

#if INSTRO_USE_CLANG
#include "lib/ClangTestSupport.h"
static llvm::cl::OptionCategory instroTool("InstRO Clang Test");
#endif

#include "instro/utility/Logger.h"
#include "instro/utility/Environment.h"

#include <iostream>
/**
 * This is the TestInstrumentor implementation.
 *
 * It expects a file with the expected items, one per line, exported to the environment variable
 * INSTRO_TEST_INPUT_FILENAME. The check is not performed on the fully qualified filename (as returned in the
 * Construct's identifier) but only the filename.
 * Fully qualified path to the input file is strongly preferred.
 *
 */
#define CDEBUG 1
/**
 * Actual instrumentor
 *
 * This instrumentor uses the ConstructTraitSelector to select each of
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
try {
#if INSTRO_USE_ROSE
	using InstrumentorType = RoseTest::RoseTestInstrumentor;
	InstrumentorType instrumentor(argc, argv);
#endif
#if INSTRO_USE_CLANG
	using InstrumentorType = ClangTest::ClangTestInstrumentor;
	InstrumentorType instrumentor(argc, argv, instroTool);
#endif

	auto factory = instrumentor.getFactory();

	std::string filename = InstRO::Utility::getEnvironmentVariable("INSTRO_TEST_INPUT_FILENAME");

	auto ctFuncLvlSelector = factory->createConstructTraitSelector(InstRO::Core::ConstructTraitType::CTFunction);
	auto ctLoopLvlSelector = factory->createConstructTraitSelector(InstRO::Core::ConstructTraitType::CTLoopStatement);
	auto ctStmtLvlSelector = factory->createConstructTraitSelector(InstRO::Core::ConstructTraitType::CTStatement);
	auto ctCondLvlSelector =
			factory->createConstructTraitSelector(InstRO::Core::ConstructTraitType::CTConditionalStatement);
	auto ctScopeLvlSelector = factory->createConstructTraitSelector(InstRO::Core::ConstructTraitType::CTScopeStatement);
	auto ctExprLvlSelector = factory->createConstructTraitSelector(InstRO::Core::ConstructTraitType::CTExpression);
	auto ctOmpLvlSelector = factory->createConstructTraitSelector(InstRO::Core::ConstructTraitType::CTOpenMPStatement);

#ifdef CDEBUG
	factory->createConstructPrinterAdapter(ctFuncLvlSelector);
	std::cout << " ------ \n";
	factory->createConstructPrinterAdapter(ctStmtLvlSelector);
#endif

	// sink, so we ignore the returned Pass *
	factory->createTestAdapter(ctFuncLvlSelector, "CTFunctionSelector", filename);
	factory->createTestAdapter(ctLoopLvlSelector, "CTLoopSelector", filename);
	factory->createTestAdapter(ctStmtLvlSelector, "CTStatementSelector", filename);
	factory->createTestAdapter(ctCondLvlSelector, "CTConditionalSelector", filename);
	factory->createTestAdapter(ctScopeLvlSelector, "CTScopeSelector", filename);
	factory->createTestAdapter(ctExprLvlSelector, "CTExpressionSelector", filename);
	factory->createTestAdapter(ctOmpLvlSelector, "CTOpenMPSelector", filename);

	instrumentor.apply();

	// Returns false if everything was fine, true otherwise
	return instrumentor.testFailed();
} catch (std::string &s) {
	std::cout << s << std::endl;
	return -1;
}
}
