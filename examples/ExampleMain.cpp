#define USING_EXAMPLE
#include "instro.h"

#include <vector>
#include <string>
#include <iostream>


int main(int argc, char** argv) {
try {
	InstRO::ExampleInstrumentor* instro = new InstRO::ExampleInstrumentor();
	
	auto aFactory = instro->getFactory();

	std::vector<std::string> filterRules;
	filterRules.push_back("main");
	auto aPass = aFactory->createIdentifierMatcherSelector(filterRules);
	auto bPass = aFactory->createIdentifierMatcherSelector(filterRules);

	auto compound = aFactory->createBooleanOrSelector(aPass, bPass);

	auto adapter = aFactory->createGPIAdapter(compound);

	instro->init();
	instro->apply();

	instro->finalize();
	} catch (...){
		std::cerr << "An exception was thrown." << std::endl;
	}

	return 0;
}
