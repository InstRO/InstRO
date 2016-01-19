#include "instro/core/Pass.h"
#include "instro/utility/exception.h"
#include "instro/utility/Logger.h"
#include "instro/core/ChannelConfiguration.h"

InstRO::Core::ConstructSet *InstRO::Core::ChannelConfiguration::operator[](int pos) const {
	logIt(DEBUG) << "requesting input for channel " << pos << std::endl;
	auto p = inputChannelPasses.at(pos);
	return p->getOutput();
}

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
