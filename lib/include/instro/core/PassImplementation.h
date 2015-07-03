#ifndef INSTRO_CORE_PASSIMPLEMENTATION_H
#define INSTRO_CORE_PASSIMPLEMENTATION_H

#include <unordered_map>

#include "instro/core/ConstructSet.h"

namespace InstRO {
// We need a forward declaration of Pass, since Pass and PassImplementation are circular
class Pass;

namespace Core {
namespace Support {
/*
 * Support class to hand the inputs from the pass manager to the
 * PassImplementation
 */
class InputAggregation {
 public:
	InputAggregation(){};
	InputAggregation(std::unordered_map<InstRO::Pass *, ConstructSet *> m) : pToCMap(m) {}
	ConstructSet *getConstructSet(InstRO::Pass *p) { return pToCMap[p]; }

 private:
	// CI: Us the Pass as a AccessHandle to the RespectiveConstruct set
	std::unordered_map<InstRO::Pass *, InstRO::Core::ConstructSet *> pToCMap;
};
}

class ChannelConfiguration {
 protected:
	std::vector<InstRO::Pass *> inputChannelPasses;
	std::unordered_map<Pass *, InstRO::Core::ContstructLevelEnum> inputChannelMin;
	std::unordered_map<Pass *, InstRO::Core::ContstructLevelEnum> inputChannelMax;

 public:
	// CI: Empty Configuration - No Input Passes used
	ChannelConfiguration(){};

	ChannelConfiguration(Pass *p1) {
		inputChannelPasses.push_back(p1);
		inputChannelMin[p1] = InstRO::Core::ContstructLevelEnum::CLMin;
		inputChannelMax[p1] = InstRO::Core::ContstructLevelEnum::CLMax;
	}

	template <class... PassList>
	ChannelConfiguration(Pass *p1, PassList... passes) {
		inputChannelPasses.insert(inputChannelPasses.begin(), {p1, passes...});
	}

	void setConstructLevel(Pass *inputPass, InstRO::Core::ContstructLevelEnum minLevel,
												 InstRO::Core::ContstructLevelEnum maxLevel) {
		inputChannelMin[inputPass] = minLevel;
		inputChannelMax[inputPass] = maxLevel;
	}
	InstRO::Core::ContstructLevelEnum getMinConstructLevel(Pass *inputPass) { return inputChannelMin[inputPass]; }
	InstRO::Core::ContstructLevelEnum getMaxConstructLevel(Pass *inputPass) { return inputChannelMax[inputPass]; }
	std::vector<InstRO::Pass *> const getPasses() { return inputChannelPasses; };
};

/*
 * This class is the user-interface to create his own pass.
 * One needs to inherit from this class and implement a compiler dependent pass.
 * Using the getInput(passId) one can retrieve the ConstructSet of one of the
 * predecessors.
 */

class PassImplementation {
 private:
	Core::ChannelConfiguration cfg;

 protected:
	Core::ChannelConfiguration &getChannelCFG() { return cfg; }

 public:
	Core::ChannelConfiguration channelCFG() { return cfg; }

	PassImplementation() = delete;

	PassImplementation(Core::ChannelConfiguration cfg) : cfg(cfg) {}

	virtual void init() = 0;
	virtual void execute() = 0;
	virtual void finalize() = 0;
	virtual void releaseOutput() = 0;
	virtual Core::ConstructSet *getOutput() = 0;

	/*	void setInputAggregation(InstRO::Core::Support::InputAggregation ia) {
			this->ia = ia;
		}*/

	InstRO::Core::ConstructSet *getInput(Pass *pId);

 protected:
 private:
	// Testing where this is used	InstRO::Core::Support::InputAggregation ia;
};
}
}
#endif
