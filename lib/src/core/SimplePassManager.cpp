#include "instro/core/Singleton.h"
#include "instro/core/Instrumentor.h"
#include "instro/core/SimplePassManager.h"
#include "instro/core/ConstructSet.h"
#include "instro/utility/Logger.h"

void InstRO::PassManagement::SimplePassManager::registerPass(Pass *currentPass) {
	passList.push_back(currentPass);
}

int InstRO::PassManagement::SimplePassManager::execute() {
	logIt(INFO) << "InstRO::PassManagement::SimplePassManager::execute()" << std::endl;

	for (Pass *pass : passList) {
		pass->initPass();
	}

	int passCount = 1;

	for (Pass *pass : passList) {
		logIt(INFO) << "Executing pass [" << passCount << "]:\t" << pass->passName() << std::endl;
		logIt(DEBUG) << "\tInput dependencies:\t" << hasInputDependencies(pass) << "\n\tOutput dependencies:\t"
								 << hasOutputDependencies(pass) << std::endl;

		for (auto &i : getPredecessors(pass)) {
			// CI: do we have to perform some form of elevation
			if (i->getOutput()->getMinConstructLevel() < pass->getMinInputLevelRequirement(i) ||
					i->getOutput()->getMaxConstructLevel() > pass->getMaxInputLevelRequirement(i)) {

				logIt(WARN) << " [WARN] construct level mismatch "
						<< "\texpected " << pass->getMinInputLevelRequirement(i) << " - "
						<< pass->getMaxInputLevelRequirement(i) << std::endl
						<< "\tprovided " << i->getOutput()->getMinConstructLevel() << " - "
						<< i->getOutput()->getMaxConstructLevel() << std::endl;

				// We need to cast the construct set
				Core::ConstructSet originalConstructSet = *(i->getOutput());
				Core::ConstructTraitType cropMin = Core::ConstructTraitType::CTMin;
				Core::ConstructTraitType cropMax = Core::ConstructTraitType::CTMax;

				if (InstRO::getInstrumentorInstance()->getConstructLoweringPolicyCrop()) {
					cropMax = pass->getMaxInputLevelRequirement(i);
				}

				if (InstRO::getInstrumentorInstance()->getConstructRaisingPolicyCrop()) {
					cropMin = pass->getMinInputLevelRequirement(i);
				}
				
				auto copy = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator()->crop(originalConstructSet,
																																																	 cropMin, cropMax);
				if (InstRO::getInstrumentorInstance()->getConstructRaisingPolicyElevate()) {
					copy = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator()->raise(
							copy, pass->getMinInputLevelRequirement(i));
				}

				if (InstRO::getInstrumentorInstance()->getConstructLoweringPolicyElevate()) {
					copy = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator()->lower(
							copy, pass->getMinInputLevelRequirement(i));
				}

				auto newCS = std::make_unique<Core::ConstructSet>(copy);
				pass->overrideInput(i, std::move(newCS));
			}
		}
		pass->executePass();
	}

	// release ConstructSet and finalize
	for (Pass *pass : passList) {
		pass->finalizePass();
	}

	return 0;
}
