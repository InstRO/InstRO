#ifndef INSTRO_PASS_SELECTOR_ELEVATORSELECTOR_H
#define INSTRO_PASS_SELECTOR_ELEVATORSELECTOR_H

#include "instro/core/ConstructSet.h"
#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Selector {

class ConstructElevatorSelector : public InstRO::Core::PassImplementation {
public:
	ConstructElevatorSelector(Pass* inputPass, InstRO::Core::ConstructTraitType targetLevel) :
			InstRO::Core::PassImplementation(InstRO::Core::ChannelConfiguration(inputPass)),
					inputPass(inputPass), targetLevel(targetLevel) {
	}

	~ConstructElevatorSelector() {}

 protected:
	Pass* inputPass;
	InstRO::Core::ConstructTraitType targetLevel;
};

class ConstructRaisingElevator : public ConstructElevatorSelector {
public:
	ConstructRaisingElevator(Pass* inputPass, InstRO::Core::ConstructTraitType targetLevel) :
			ConstructElevatorSelector(inputPass, targetLevel) {
	}

	virtual void execute() override;
};

class ConstructLoweringElevator: public ConstructElevatorSelector {
public:
	ConstructLoweringElevator(Pass* inputPass, InstRO::Core::ConstructTraitType targetLevel) :
			ConstructElevatorSelector(inputPass, targetLevel) {
	}

	void execute() override;
};


class ConstructCroppingElevator: public ConstructElevatorSelector {
public:
	ConstructCroppingElevator(Pass* inputPass, InstRO::Core::ConstructTraitType minLevel, InstRO::Core::ConstructTraitType maxLevel) :
			// uses targetLevel as minLevel
			ConstructElevatorSelector(inputPass, minLevel), maxLevel(maxLevel) {
	}

	void execute() override;

 private:
	InstRO::Core::ConstructTraitType maxLevel;
};

}
}

#endif
