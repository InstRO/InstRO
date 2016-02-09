#define USE_ROSE
#include "instro.h"

#include <vector>
#include <string>
#include <iostream>

int main(int argc, char** argv) {
	try {
		auto instro = new InstRO::RoseInstrumentor(argc, argv);
		auto factory = instro->getFactory();

		auto mainFunction = factory->createProgramEntrySelector();
		factory->createConstructPrinterAdapter(mainFunction);

		auto functions = factory->createConstructTraitSelector(InstRO::Core::ConstructTraitType::CTFunction);
		factory->createConstructPrinterAdapter(functions);

		auto callpaths = factory->createCallpathSelector(mainFunction, functions, "callpaths.dot");
		factory->createConstructPrinterAdapter(callpaths);
		factory->createDefaultInstrumentationAdapter(callpaths);

		instro->apply();

	} catch (std::string& stringBasedException) {
		std::cerr << "Caught Exception:" << std::endl;
		std::cerr << stringBasedException << std::endl;
	}

	return 0;
}
