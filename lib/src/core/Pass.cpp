#include "instro/core/Pass.h"
#include "instro/utility/exception.h"
#include "instro/utility/Logger.h"

void InstRO::Pass::initPass() {
	if (!passInitialized && !passFinalized) {
		passImplementation->init();
		passInitialized = true;
	} else {
		raise_exception("Pass Initialized after Finalize. Not supported.");
	}
}

void InstRO::Pass::executePass() {
	assert(passImplementation);
	logIt(INFO) << "InstRO::Pass::executePass:\t calling execute on PassImplementation" << std::endl;
	if (passInitialized) {
		passImplementation->execute();
	} else {
		raise_exception("Pass not initialized.");
	}
	passExecuted = true;
}

void InstRO::Pass::finalizePass() {
	if (passInitialized && passExecuted) {
		passImplementation->finalize();
	} else {
		raise_exception("Must initialize pass first.");
	}
	passFinalized = true;
	passInitialized = false;
}
