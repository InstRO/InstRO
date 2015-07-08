#include "instro/core/ConstructSet.h"
#include "instro/rose/core/RosePassImplementation.h"

namespace InstRO {
namespace Rose {
namespace Selector {

class ConstructRaisingElevator : public InstRO::Rose::RosePassImplementation {
 protected:
	Pass *input;
	ConstructRaisingElevator() = delete;
	std::unique_ptr<Core::ConstructSet> output;
	InstRO::Core::ConstructLevelType minLevel;

 public:
	ConstructRaisingElevator(Pass *inputArg, InstRO::Core::ConstructLevelType clArg)
			: RosePassImplementation(Core::ChannelConfiguration(inputArg)), input(inputArg), minLevel(clArg){};
	virtual void init() override;
	virtual void execute() override;
	virtual void finalize() override;
	virtual void releaseOutput() override;
	virtual Core::ConstructSet *getOutput() override;

 protected:
};

class ConstructLoweringElevator : public InstRO::Rose::RosePassImplementation {
 protected:
	Pass *input;
	ConstructLoweringElevator() = delete;
	std::unique_ptr<Core::ConstructSet> output;
	InstRO::Core::ConstructLevelType maxLevel;

 public:
	ConstructLoweringElevator(Pass *inputArg, InstRO::Core::ConstructLevelType clArg)
			: RosePassImplementation(Core::ChannelConfiguration(inputArg)), input(inputArg), maxLevel(clArg){};
	virtual void init() override;
	virtual void execute() override;
	virtual void finalize() override;
	virtual void releaseOutput() override;
	virtual Core::ConstructSet *getOutput() override;

 protected:
};
}
}
}