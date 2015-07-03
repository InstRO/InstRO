#include "instro/core/Pass.h"

void InstRO::Pass::initPass() {
	if (!passInitialized && !passFinalized){
		passImplementation->init();
		passInitialized = true;
	}
	else
#ifdef __EXCEPTIONS
		throw std::string("Pass Initialized after Finalize! Not Supported!");
#else
		std::cerr << "Pass Initialized after Finalize! Not Supported!" << std::endl;
#endif
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
	if (passInitialized && passExecuted)
		passImplementation->finalize();
	else
#ifdef __EXCEPTIONS
		throw std::string("Must Initialize Pass First!");
#else
		std::cerr << "Pass: Must Initialize Pass First" << std::endl;
#endif
	passFinalized = true;
	passInitialized = false;
}

