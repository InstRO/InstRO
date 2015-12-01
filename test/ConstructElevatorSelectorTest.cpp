
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
	using CTrait = InstRO::Core::ConstructTraitType;

	InstrumentorType instrumentor(argc, argv);
	auto factory = instrumentor.getFactory();

	std::string filename = InstRO::Utility::getEnvironmentVariable("INSTRO_TEST_INPUT_FILENAME");

	auto funcLvlSelector = factory->createConstructClassSelector(CTrait::CTFunction);
	auto funcToStmtLvlSelector = factory->createConstructLoweringElevator(funcLvlSelector, CTrait::CTStatement);
	factory->createTestAdapter(funcToStmtLvlSelector, "Func2StmtLevelSelector", filename);

	auto stmtLvlSelector = factory->createConstructClassSelector(CTrait::CTStatement);
	auto stmtToFuncLvlSelector = factory->createConstructRaisingElevator(stmtLvlSelector, CTrait::CTFunction);
	factory->createTestAdapter(stmtToFuncLvlSelector, "Stmt2FuncLevelSelector", filename);

	auto funcToExprLvlSelector = factory->createConstructLoweringElevator(funcLvlSelector, CTrait::CTExpression);
	factory->createTestAdapter(funcToExprLvlSelector, "Func2ExprLevelSelector", filename);

	auto stmtClassSelector = factory->createConstructClassSelector(CTrait::CTStatement);
	auto exprClassSelector = factory->createConstructClassSelector(CTrait::CTExpression);
	auto booleanOrSelector = factory->createBooleanOrSelector(stmtClassSelector, exprClassSelector);

	auto wpToExprStmtSelector =
			factory->createConstructCroppingElevator(booleanOrSelector, CTrait::CTExpression, CTrait::CTStatement);
	auto wpToStmtFuncSelector =
			factory->createConstructCroppingElevator(booleanOrSelector, CTrait::CTStatement, CTrait::CTFunction);

	factory->createTestAdapter(wpToExprStmtSelector, "wpToExprStmtSelector", filename);
	factory->createTestAdapter(wpToStmtFuncSelector, "wpToStmtFuncSelector", filename);

#ifdef DEBUG
	factory->createConstructPrinter(funcToStmtLvlSelector);
	factory->creaeConstructPrinter(stmtToFuncLvlSelector);
#endif

	instrumentor.apply();

	return instrumentor.testFailed();
#endif

#ifdef USE_CLANG
	logIt(ERROR) << "Not implemented yet!" << std::endl;
	return 0;
#endif
}
