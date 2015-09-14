#include "instro/core/ConstructSet.h"
#include "instro/rose/core/RosePassImplementation.h"

namespace InstRO {
namespace Rose {
namespace Selector {

class ConstructRaisingElevator : public InstRO::Rose::RosePassImplementation {
 protected:
	Pass *input;
	ConstructRaisingElevator() = delete;
	InstRO::Core::ConstructSet output;
	InstRO::Core::ConstructTraitType minLevel;

 public:
	ConstructRaisingElevator(Pass *inputArg, InstRO::Core::ConstructTraitType clArg)
			: RosePassImplementation(InstRO::Core::ChannelConfiguration(inputArg)), input(inputArg), minLevel(clArg){};
	virtual void init() override;
	virtual void execute() override;
	virtual void finalize() override;
	virtual void releaseOutput() override;
	virtual InstRO::Core::ConstructSet *getOutput() override;

 protected:
};

class ConstructLoweringElevator : public InstRO::Rose::RosePassImplementation {
 protected:
	Pass *input;
	ConstructLoweringElevator() = delete;
	InstRO::Core::ConstructSet output;
	InstRO::Core::ConstructTraitType maxLevel;

 public:
	ConstructLoweringElevator(Pass *inputArg, InstRO::Core::ConstructTraitType clArg)
			: RosePassImplementation(InstRO::Core::ChannelConfiguration(inputArg)), input(inputArg), maxLevel(clArg){};
	virtual void init() override;
	virtual void execute() override;
	virtual void finalize() override;
	virtual void releaseOutput() override;
	virtual InstRO::Core::ConstructSet *getOutput() override;

 protected:
};
}
}
}
