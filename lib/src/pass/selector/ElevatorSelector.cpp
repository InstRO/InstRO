#include "instro/core/Instrumentor.h"
#include "instro/core/ConstructSet.h"
#include "instro/core/Singleton.h"
#include "instro/tooling/AnalysisInterface.h"

#include "instro/pass/selector/ElevatorSelector.h"

namespace InstRO {
namespace Selector {

void ConstructRaisingElevator::execute() {
	auto constructElevator = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator();
	output = std::make_unique<InstRO::Core::ConstructSet>(constructElevator->raise(inputPass->getOutput(), targetLevel));
}

void ConstructLoweringElevator::execute() {
	auto constructElevator = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator();
	output = std::make_unique<InstRO::Core::ConstructSet>(constructElevator->lower(inputPass->getOutput(), targetLevel));
}

void ConstructCroppingElevator::execute() {
	auto constructElevator = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator();
	output = std::make_unique<InstRO::Core::ConstructSet>(
			constructElevator->crop(inputPass->getOutput(), targetLevel, maxLevel));	// targetLevel is minLevel
}

}
}
