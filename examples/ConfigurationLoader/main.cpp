
#include "instro.h"

#include "instro/utility/ConfigurationLoader.h"
#include "instro/utility/MemoryManagement.h" // provide make_unique

#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>

int main(int argc, char** argv) {
	try {
		auto instro = new InstRO::RoseInstrumentor(argc, argv);

		auto aFactory = instro->getFactory();
		InstRO::Utility::ConfigurationLoader configLoader(
				std::make_unique<InstRO::Utility::RoseConfigurationPassRegistry>(aFactory));

		char* jsonFile = std::getenv("INSTRO_CONFIG");
		if (jsonFile) {
			configLoader.load(jsonFile);
		} else {
			configLoader.load("input.json");
		}

		instro->apply();

	} catch (std::string& stringBasedException) {
		std::cerr << "Caught Exception:" << std::endl;
		std::cerr << stringBasedException << std::endl;
	}

	return 0;
}
