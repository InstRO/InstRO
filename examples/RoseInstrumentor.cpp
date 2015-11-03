#define USING_ROSE
#include "instro.h"

#include <vector>
#include <string>
#include <iostream>

// using namespace InstRO;

int main(int argc, char** argv) {
	try {
		auto instro = new InstRO::RoseInstrumentor(argc, argv);
		auto aFactory = dynamic_cast<InstRO::Rose::RosePassFactory*>(instro->getFactory());

		auto functions = aFactory->createConstructClassSelector(InstRO::Core::ConstructTraitType::CTFunction);
		aFactory->createConstructPrinter(functions);

		auto statements = aFactory->createConstructLoweringElevator(functions, InstRO::Core::ConstructTraitType::CTStatement);
		aFactory->createConstructPrinter(statements);

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
