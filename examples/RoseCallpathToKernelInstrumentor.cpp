#define USE_ROSE
#include "instro.h"

#include <vector>
#include <string>
#include <iostream>

int main(int argc, char** argv) {
	try {
		InstRO::RoseInstrumentor instro(argc, argv);
		auto factory = instro.getFactory();

		// Gives us the main function
		auto entry = factory->createProgramEntrySelector();

		// Does a name matching
		auto kernel = factory->createIdentifierMatcherSelector({std::string("#Kernel#")});
		// Gives all the functions in the project
		auto functions = factory->createConstructTraitSelector(InstRO::Core::ConstructTraitType::CTFunction);
		// Results in only those functions which match the name "#kernel#"
		auto kernelFunctions = factory->createBooleanAndSelector(kernel, functions);

		// Keeps everything which is contained on the path
		auto callpathToKernel = factory->createCallpathSelector(entry, kernelFunctions);

		factory->createDefaultInstrumentationAdapter(callpathToKernel);

		instro.apply();

	} catch (std::string& stringBasedException) {
		std::cerr << "Caught Exception:" << std::endl;
		std::cerr << stringBasedException << std::endl;
	}

	return 0;
}
