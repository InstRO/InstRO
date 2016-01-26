#ifndef INSTRO_PASS_SELECTOR_ELEVATORSELECTOR_H
#define INSTRO_PASS_SELECTOR_ELEVATORSELECTOR_H

#include "instro/core/ConstructSet.h"
#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Selector {

class ConstructElevatorSelector : public InstRO::Core::PassImplementation {
 public:
	ConstructElevatorSelector(InstRO::Core::ConstructTraitType targetLevel) : targetLevel(targetLevel) {}

	~ConstructElevatorSelector() {}

 protected:
	InstRO::Core::ConstructTraitType targetLevel;
};

class ConstructRaisingElevator : public ConstructElevatorSelector {
 public:
	ConstructRaisingElevator(InstRO::Core::ConstructTraitType targetLevel) : ConstructElevatorSelector(targetLevel) {}

	virtual void execute() override;
};

class ConstructLoweringElevator : public ConstructElevatorSelector {
 public:
	ConstructLoweringElevator(InstRO::Core::ConstructTraitType targetLevel) : ConstructElevatorSelector(targetLevel) {}

	void execute() override;
};

class ConstructCroppingElevator : public ConstructElevatorSelector {
 public:
	ConstructCroppingElevator(InstRO::Core::ConstructTraitType minLevel, InstRO::Core::ConstructTraitType maxLevel)
			:	// uses targetLevel as minLevel
				ConstructElevatorSelector(minLevel),
				maxLevel(maxLevel) {}

	void execute() override;

 private:
	InstRO::Core::ConstructTraitType maxLevel;
};
}
}

#endif
