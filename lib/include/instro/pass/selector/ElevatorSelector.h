
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

	void init() override {}

	void finalize() override {}

	void releaseOutput() override { output->clear();	}

	InstRO::Core::ConstructSet *getOutput() override { return output.get();	}

 protected:
	Pass* inputPass;
	InstRO::Core::ConstructTraitType targetLevel;
	std::unique_ptr<InstRO::Core::ConstructSet> output;
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