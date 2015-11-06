#define USING_ROSE
#include "instro.h"

#include <vector>
#include <string>
#include <iostream>

// using namespace InstRO;

int main(int argc, char** argv) {
	try {
		auto instro = new InstRO::RoseInstrumentor(argc, argv);
		auto aFactory = instro->getFactory();

		auto mainFunction = aFactory->createProgramEntrySelector();
		aFactory->createConstructPrinter(mainFunction);

		auto functions = aFactory->createConstructClassSelector(InstRO::Core::ConstructTraitType::CTFunction);
		aFactory->createConstructPrinter(functions);

//		auto statements = aFactory->createConstructLoweringElevator(functions, InstRO::Core::ConstructTraitType::CTStatement);
//		aFactory->createConstructPrinter(statements);

		auto intersect = aFactory->createBooleanAndSelector(mainFunction, functions);
		aFactory->createConstructPrinter(intersect);

		intersect = aFactory->createBooleanOrSelector(mainFunction, functions);
		aFactory->createConstructPrinter(intersect);

		intersect = aFactory->createBooleanXorSelector(mainFunction, functions);
		aFactory->createConstructPrinter(intersect);

		intersect = aFactory->createBooleanMinusSelector(functions, mainFunction);
		aFactory->createConstructPrinter(intersect);

		auto paths = aFactory->createCallpathSelector(mainFunction, functions, "cps.dot");
		aFactory->createConstructPrinter(paths);

//		aFactory->createMatthiasZoellnerLoopInstrumentationAdapter(functions);

		instro->init();
		instro->apply();
		instro->finalize();

	} catch (std::string& stringBasedException) {
		std::cerr << "Caught Exception:" << std::endl;
		std::cerr << stringBasedException << std::endl;
		std::cerr.flush();
	}

	return 0;
}
