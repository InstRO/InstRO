#ifndef INSTRO_CORE_PASSIMPLEMENTATION_H
#define INSTRO_CORE_PASSIMPLEMENTATION_H
#include <initializer_list>
#include <unordered_map>
// Pass.h is included at the End of this file. We use forward declarations to break the circle Pass - PassImplementation
// - Pass
// #include "instro/core/Pass.h"
#include "instro/core/ConstructSet.h"
#include "instro/core/Helper.h"

namespace InstRO {
// We need a forward declaration of Pass, since Pass and PassImplementation are circular
class Pass;

namespace Core {
namespace Support {
/*
 * Support class to hand the inputs from the pass manager to the
 * PassImplementation
 */
/*
class InputAggregation {
public:
InputAggregation(){};
InputAggregation(std::unordered_map<InstRO::Pass *, ConstructSet *> m) : pToCMap(m) {}
ConstructSet *getConstructSet(InstRO::Pass *p) { return pToCMap[p]; }

private:
// CI: Us the Pass as a AccessHandle to the RespectiveConstruct set
std::unordered_map<InstRO::Pass *, InstRO::Core::ConstructSet *> pToCMap;
};*/
}

class ChannelConfiguration {
 protected:
	std::vector<Pass *> inputChannelPasses;
	std::unordered_map<Pass *, InstRO::Core::ConstructTraitType> inputChannelMin;
	// std::unordered_map<Pass *, ContstructTraitType> inputChannelMin;
	std::unordered_map<Pass *, InstRO::Core::ConstructTraitType> inputChannelMax;

 public:
	// CI: Empty Configuration - No Input Passes used
	ChannelConfiguration(){};

	ChannelConfiguration(Pass *p1) {
		inputChannelPasses.push_back(p1);
		inputChannelMin[p1] = ::InstRO::Core::ConstructTraitType::CTMin;
		inputChannelMax[p1] = ::InstRO::Core::ConstructTraitType::CTMax;
	}

	template <class... PassList>
	ChannelConfiguration(Pass *p1, PassList... passes) {
		inputChannelPasses.insert(inputChannelPasses.begin(), {p1, passes...});
	}

	template <class Iterator>
	ChannelConfiguration(Iterator begin, Iterator end, InstRO::Core::ConstructTraitType minLevel, InstRO::Core::ConstructTraitType maxLevel) {
        for (Iterator iter = begin; iter != end; ++iter) {
            inputChannelPasses.push_back(*iter);
            inputChannelMin[*iter] = minLevel;
            inputChannelMax[*iter] = maxLevel;
        }
	}

	struct PassMinMaxSequenceHelper {
		::InstRO::Pass *pass;
		::InstRO::Core::ConstructTraitType min;
		::InstRO::Core::ConstructTraitType max;
	};

	template <class... PassList>
	ChannelConfiguration(std::initializer_list<PassMinMaxSequenceHelper> passes) {
		for (auto tripel : passes) {
			inputChannelPasses.push_back(tripel.pass);
			inputChannelMin[tripel.pass] = tripel.min;
			inputChannelMax[tripel.pass] = tripel.max;
		}
	}

	void setConstructLevel(Pass *inputPass, ::InstRO::Core::ConstructTraitType minLevel,
												 ::InstRO::Core::ConstructTraitType maxLevel) {
		inputChannelMin[inputPass] = minLevel;
		inputChannelMax[inputPass] = maxLevel;
	}
	::InstRO::Core::ConstructTraitType getMinConstructLevel(Pass *inputPass) { return inputChannelMin[inputPass]; }
	::InstRO::Core::ConstructTraitType getMaxConstructLevel(Pass *inputPass) { return inputChannelMax[inputPass]; }
	std::vector<::InstRO::Pass *> const getPasses() { return inputChannelPasses; };
};

/*
 * This class is the user-interface to create his own pass.
 * One needs to inherit from this class and implement a compiler dependent pass.
 * Using the getInput(passId) one can retrieve the ConstructSet of one of the
 * predecessors.
 */
class PassImplementation {
 public:
	PassImplementation(ChannelConfiguration channelConfig)
			: channelConfig(channelConfig), collisionSet(std::make_unique<ConstructSet>()) {}
	PassImplementation() = delete;
	virtual ~PassImplementation() {}

	virtual void init() = 0;
	virtual void execute() = 0;
	virtual void finalize() = 0;
	virtual void releaseOutput() = 0;

	ChannelConfiguration getChannelConfig() { return channelConfig; }
	virtual ConstructSet *getOutput() = 0;
	ConstructSet *getInput(Pass *pId);

	ConstructSet *getCollisionSet() { return collisionSet.get(); };

 private:
	ChannelConfiguration channelConfig;

 protected:
	ChannelConfiguration &getChannelCFG() { return channelConfig; }

	// This set is used to track alterations to the AST and notify which nodes have been invalidated
	std::unique_ptr<ConstructSet> collisionSet;
};

}	// namespace Core
}	// namespace InstRO

#endif

#include "instro/core/Pass.h"
