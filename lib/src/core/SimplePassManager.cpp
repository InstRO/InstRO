#include "instro/core/Singleton.h"
#include "instro/core/Instrumentor.h"
#include "instro/core/SimplePassManager.h"
#include "instro/core/ConstructSet.h"

void InstRO::PassManagement::SimplePassManager::registerPass(Pass *currentPass) {
	// CI: Create a new pass envelope to store the dependencies of this pass.
	PassEnvelope *newPass = new PassEnvelope(currentPass);
	std::vector<Pass *> inputs = currentPass->getInputPasses();
	passList.push_back(newPass);
	//	newPass->predecessors=currentPass->getInputPasses();
	// for all predecessors inquired the pass dependencies
	for (auto &i : inputs) {
		if (i == NULL)
			continue;
		Core::ConstructTraitType maxInputLevel = currentPass->getMaxInputLevelRequirement(i);
		//	Core::ConstructLevelType minOutputLevelProvided = i->getOutputLevel();

		addDependency(i, currentPass);
	}
}

void InstRO::PassManagement::SimplePassManager::setExecuter(InstRO::PassManagement::PassExecuter *executer) {
	this->executer = executer;
}

int InstRO::PassManagement::SimplePassManager::execute() {
	std::cout << "InstRO::PassManagement::SimplePassManager::execute()" << std::endl;

	for (PassEnvelope *passContainer : passList) {
		// Allow the Pass to Initialize iself. E.g. start reading input data from
		// files, allocated named input fields, etc.

		passContainer->pass->initPass();
	}
	int passCount = 1;

	for (PassEnvelope *passEnvelope : passList) {
		std::cout << "\texecuting pass (" << passCount << "):\t" << passEnvelope->pass->passName() << std::endl;

		//	std::vector<std::unique_ptr<InstRO::Core::ConstructSet> > tempConstructSets;

		// check if some input needs to be explicitly elevated
		std::unordered_map<InstRO::Pass *, InstRO::Core::ConstructSet *> mymap;
		for (auto &i : getPredecessors(passEnvelope)) {
			// CI: do we have to perform some form of elevation
			if (i->getOutput()->getMinConstructLevel() < passEnvelope->pass->getMinInputLevelRequirement(i) ||
					i->getOutput()->getMaxConstructLevel() > passEnvelope->pass->getMaxInputLevelRequirement(i)) {
				std::cout << "\t construct level missmatch " << std::endl;
				// We need to cast the construct set
				// Any of the various elevators or crop functions returns a new unique_ptr. As result the copies will be cleaned
				Core::ConstructSet originalConstructSet = *(i->getOutput());
				Core::ConstructTraitType cropMin = Core::ConstructTraitType::CTMin;
				Core::ConstructTraitType cropMax = Core::ConstructTraitType::CTMax;

				if (InstRO::getInstrumentorInstance()->getConstructLoweringPolicyCrop())
					cropMax = passEnvelope->pass->getMaxInputLevelRequirement(i);
				if (InstRO::getInstrumentorInstance()->getConstructRaisingPolicyCrop())
					cropMin = passEnvelope->pass->getMinInputLevelRequirement(i);
				auto copy =
						InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator()->crop(originalConstructSet,
																																													 cropMin, cropMax);

				if (InstRO::getInstrumentorInstance()->getConstructRaisingPolicyElevate())
					copy = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator()->raise(
							copy, passEnvelope->pass->getMinInputLevelRequirement(i));

				if (InstRO::getInstrumentorInstance()->getConstructLoweringPolicyElevate())
					copy = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator()->lower(
							copy, passEnvelope->pass->getMinInputLevelRequirement(i));
				auto newCS=std::make_unique<Core::ConstructSet>(copy);
				passEnvelope->pass->overrideInput(i, std::move(newCS));
				//				tempConstructSets.push_back(copy);
			}

			// I introduced the concept of an input aggregation for the pass
			// implementation. This entity encapsulates the Construct sets for all the
			// predeccor Passes. So the PassManager sets the according construct sets
			// inside the input aggregation.

			//		std::cout << "Key in ConstructSetOverride map " << i << std::endl;
			//		mymap[i] = i->getPassImplementation()->getOutput();
		}
		//	InstRO::Core::Support::InputAggregation ia(mymap);

		// After this we can enable the input, and pass impls can query for the
		// result of pass p
		// passEnvelope->pass->getPassImplementation()->setInputAggregation(ia);

		// 1rst enable the input for the current pass. Since this is the basic
		// pass manager, the sequence is linear with no intelligence. Hence, all
		// preceeding passes must have already completed.
		// CI refactorying|		passEnvelope->pass->setInputEnabled();

		// 2nd: Allow the pass to initlialize its internal state, such that it can
		// execute. This allows, e.g. to allocate large amounts of memory. Or
		// preprocessing input. or whatever. However, modification or selection is
		// not allowed.
		//	passEnvelope->pass->initPass();

		// 3rd: Execute the pass (using delegate, since the delegate know what to do
		// exactly)
		//		std::cout << "now invoking executePass" << std::endl;
		passEnvelope->pass->executePass();

		// 4th: Tell the pass to finalize. It is supposed to release memory, close
		// files, etc. However, the output set must be maintained unitl
		// disableOutput is called
		//	passEnvelope->pass->finalizePass();

		// 5th: Disable input and deallocated potentially create construct sets
		// CI refactorying|		passEnvelope->pass->setInputDisabled();
		//	for (auto &i : tempConstructSets) delete i;
	}
	for (PassEnvelope *passContainer : passList) {
		// disable output for all passes. This allows to release the output
		// construct set
		passContainer->pass->finalizePass();
	}
	return 0;
}
