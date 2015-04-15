#include "instro/core/Pass.h"

void InstRO::Pass::initPass() {
	if (inputReady)
		passImplementation->init();
	else
#ifdef __EXCEPTIONS
		throw std::string("Input not ready!");
#else
		std::cerr << "Pass: Input not ready" << std::endl;
#endif
	passInitialized = true;
}

void InstRO::Pass::executePass() {
	std::cout << "Executing inside Pass base class" << std::endl;
	if (passInitialized)
		passImplementation->execute();
	else
#ifdef __EXCEPTIONS
		throw std::string("Pass not Initialized!");
#else
		std::cerr << "Pass: Pass not initialized" << std::endl;
#endif
	passExecuted = true;
}

void InstRO::Pass::finalizePass() {
	if (passInitialized)
		passImplementation->finalize();
	else
#ifdef __EXCEPTIONS
		throw std::string("Must Initialize Pass First!");
#else
		std::cerr << "Pass: Must Initialize Pass First" << std::endl;
#endif
	passFinalize = true;
}

void InstRO::Pass::releaseOutput() {
	if (passExecuted)
		passImplementation->releaseOutput();
	else
#ifdef __EXCEPTIONS
		throw std::string("Pass not Executed, Execute Pass First!");
#else
		std::cerr << "Pass: Pass not executed. Need to execute pass first"
							<< std::endl;
#endif
	passOutputReleased = true;
}

