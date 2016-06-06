#include "instro/Pass.h"

#include "instro/utility/exception.h"
#include "instro/utility/Logger.h"
#include "instro/core/ChannelConfiguration.h"

//// ChannelConfiguration

InstRO::Core::ConstructSet *InstRO::Core::ChannelConfiguration::getConstructSetForChannel(int channel) const {
	logIt(DEBUG) << "Requesting construct set for input channel " << channel << std::endl;
	auto p = inputChannelMap.at(channel);
	return p->getOutput();
}


//// Pass

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
		passImplementation->releaseOutput();
		passImplementation->finalize();
	} else {
		raise_exception("Must initialize pass first.");
	}
	passFinalized = true;
	passInitialized = false;
}
