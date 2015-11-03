#include "instro/core/Singleton.h"
#include "instro/core/Instrumentor.h"
#include "instro/core/SimplePassManager.h"
#include "instro/core/ConstructSet.h"
#include "instro/utility/Logger.h"

void InstRO::PassManagement::SimplePassManager::registerPass(Pass *currentPass) {
	// CI: Create a new pass envelope to store the dependencies of this pass.
	PassEnvelope *newPass = new PassEnvelope(currentPass);
	std::vector<Pass *> inputs = currentPass->getInputPasses();
	passList.push_back(newPass);

	// for all predecessors inquired the pass dependencies
	for (auto &i : inputs) {
		if (i == nullptr)
			continue;
		Core::ConstructTraitType maxInputLevel = currentPass->getMaxInputLevelRequirement(i);

		addDependency(i, currentPass);
	}
}

bool InstRO::PassManagement::SimplePassManager::createPassTraversalOder() { return true; }

int InstRO::PassManagement::SimplePassManager::execute() {
	logIt(INFO) << "InstRO::PassManagement::SimplePassManager::execute()" << std::endl;

	for (PassEnvelope *passContainer : passList) {
		// Allow the Pass to Initialize iself. E.g. start reading input data from
		// files, allocated named input fields, etc.
		passContainer->pass->initPass();
	}

	int passCount = 1;

	for (PassEnvelope *passEnvelope : passList) {
		logIt(INFO) << "\texecuting pass (" << passCount << "):\t" << passEnvelope->pass->passName() << std::endl;

		for (auto &i : getPredecessors(passEnvelope)) {
			// CI: do we have to perform some form of elevation
			if (i->getOutput()->getMinConstructLevel() < passEnvelope->pass->getMinInputLevelRequirement(i) ||
					i->getOutput()->getMaxConstructLevel() > passEnvelope->pass->getMaxInputLevelRequirement(i)) {
				logIt(WARN) << "\t construct level mismatch " << std::endl;

				// We need to cast the construct set
				// Any of the various elevators or crop functions returns a new unique_ptr. As result the copies will be cleaned
				Core::ConstructSet originalConstructSet = *(i->getOutput());
				Core::ConstructTraitType cropMin = Core::ConstructTraitType::CTMin;
				Core::ConstructTraitType cropMax = Core::ConstructTraitType::CTMax;

				if (InstRO::getInstrumentorInstance()->getConstructLoweringPolicyCrop())
					cropMax = passEnvelope->pass->getMaxInputLevelRequirement(i);
				if (InstRO::getInstrumentorInstance()->getConstructRaisingPolicyCrop())
					cropMin = passEnvelope->pass->getMinInputLevelRequirement(i);
				auto copy = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator()->crop(originalConstructSet,
																																																	 cropMin, cropMax);

				if (InstRO::getInstrumentorInstance()->getConstructRaisingPolicyElevate())
					copy = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator()->raise(
							copy, passEnvelope->pass->getMinInputLevelRequirement(i));

				if (InstRO::getInstrumentorInstance()->getConstructLoweringPolicyElevate())
					copy = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator()->lower(
							copy, passEnvelope->pass->getMinInputLevelRequirement(i));
				auto newCS = std::make_unique<Core::ConstructSet>(copy);
				passEnvelope->pass->overrideInput(i, std::move(newCS));
			}
		}
		passEnvelope->pass->executePass();
	}

	for (PassEnvelope *passContainer : passList) {
		// disable output for all passes. This allows to release the output
		// construct set
		passContainer->pass->finalizePass();
	}

	return 0;
}
