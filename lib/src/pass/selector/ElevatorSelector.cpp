#include "instro/pass/selector/ElevatorSelector.h"

#include "instro/Instrumentor.h"
#include "instro/core/ConstructSet.h"
#include "instro/core/Singleton.h"
#include "instro/tooling/AnalysisInterface.h"

namespace InstRO {
namespace Selector {

void ConstructRaisingElevator::execute() {
	auto constructElevator = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator();
	outputSet = constructElevator->raise(getInput(0), targetLevel);
}

void ConstructLoweringElevator::execute() {
	auto constructElevator = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator();
	outputSet = constructElevator->lower(getInput(0), targetLevel);
}

void ConstructCroppingElevator::execute() {
	auto constructElevator = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator();
	outputSet = constructElevator->crop(getInput(0), targetLevel, maxLevel);	// targetLevel is minLevel
}
}
}
