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

#ifdef LEGACY_LLVM
void InstRO::Pass::execute(InstRO::PassManagement::PassExecuter *executer) {
	assert(false);
	if (passInitialized)
		executer->execute(passImplementation);
	else
#ifdef __EXCEPTIONS
		throw std::string("Pass not Initialized!");
#else
		std::cerr << "Pass: Pass not initialized" << std::endl;
#endif
	passExecuted = true;
}
#endif 


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

