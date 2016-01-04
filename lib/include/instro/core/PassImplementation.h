#ifndef INSTRO_CORE_PASSIMPLEMENTATION_H
#define INSTRO_CORE_PASSIMPLEMENTATION_H
#include <initializer_list>
#include <unordered_map>
// Pass.h is included at the End of this file. We use forward declarations to break the circle Pass - PassImplementation
// - Pass
// #include "instro/core/Pass.h"
#include "instro/core/ConstructSet.h"
#include "instro/utility/MemoryManagement.h"

namespace InstRO {
// We need a forward declaration of Pass, since Pass and PassImplementation are circular
class Pass;

namespace Core {

/*
 * The class serves as interface between Pass and PassImplementation to make them independent of the actual input channel
 */
class ChannelConfiguration {
 protected:
	std::vector<Pass *> inputChannelPasses;
	std::unordered_map<Pass *, InstRO::Core::ConstructTraitType> inputChannelMin;
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

		std::vector<Pass*> allPasses = {p1, passes...};
		for (auto pass : allPasses) {
			inputChannelMin[pass] = InstRO::Core::ConstructTraitType::CTMin;
			inputChannelMax[pass] = InstRO::Core::ConstructTraitType::CTMax;
		}
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
		: channelConfig(channelConfig), collisionSet() {}
	PassImplementation() = delete;
	virtual ~PassImplementation() {}

	virtual void init() {}	// provide default impl because this is the default case
	virtual void execute() = 0;
	virtual void finalize() {}	// provide default impl because this is the default case

	virtual void releaseOutput() { outputSet.clear(); }
	ChannelConfiguration getChannelConfig() { return channelConfig; }

	virtual ConstructSet* getOutput() { return &outputSet; };
	ConstructSet* getInput(Pass *pId);

private:
	ChannelConfiguration channelConfig;

protected:
	ConstructSet outputSet;
	
	ChannelConfiguration &getChannelCFG() { return channelConfig; }

	// This set is used to track alterations to the AST and notify which nodes have been invalidated
	ConstructSet collisionSet;
	ConstructSet* getCollisionSet() { return &collisionSet; }
public:
	const ConstructSet* cgetCollisionSet() { return &collisionSet; };

};


}	// namespace Core
}	// namespace InstRO

#endif

#include "instro/core/Pass.h"
