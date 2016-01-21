#ifndef INSTRO_CORE_PASS_H
#define INSTRO_CORE_PASS_H

#include <map>
#include <string>
#include <iostream>
#include <cassert>

#include <vector>
#include <unordered_map>

#include "instro/core/ConstructSet.h"
#include "instro/core/ChannelConfiguration.h"
#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace PassManagement {
class PassManager;
}

/*
 * The Pass class is the administration class used within the pass manager.
 * It holds information necessary in order to singalize the pass implementation
 * whether it can be run or not.
 */
class Pass {
	friend InstRO::PassManagement::PassManager;

 public:
	// CI empty pass construction disallowed. Each Pass is an container for the corresponding PassImplementation Object
	// from the ToolingSpace
	Pass() = delete;
	Pass(Core::PassImplementation *pImpl, InstRO::Core::ChannelConfiguration cfg, const std::string passName)
			: passInitialized(false),
				passExecuted(false),
				passFinalized(false),
				passNameString(passName),
				channelConfig(cfg),
				passImplementation(pImpl) {
		pImpl->managingPass = this;
	}

	virtual ~Pass() {
		delete (passImplementation);
		passImplementation = nullptr;
	}
	
	// CI: Tell the pass, that is is allowed to initialize itself
	void initPass();
	
	// CI: Execute the pass, this generates the output-constructset
	void executePass();
	
	void finalizePass();

	// Query the proxy pass for its output
	Core::ConstructSet *getOutput() { return passImplementation->getOutput(); };

	// returns the ConstructSet for the input channel from
	Core::ConstructSet *getInput(Pass *from) {
		if (inputOverride.find(from) == inputOverride.end()) {
			return from->getOutput();
		} else {
			return inputOverride.find(from)->second.get();
		}
	}

	const Core::ConstructSet *getInput(int channel) const { return channelConfig[channel]; }

	// Allows to inject a ConstructSet which is returned for the pass with id from
	void overrideInput(Pass *from, std::unique_ptr<Core::ConstructSet> overrideSet) {
		std::shared_ptr<Core::ConstructSet> sharedSet(std::move(overrideSet));
		inputOverride[from] = sharedSet;
	}

	// This allows for passes to have a unique name defined by the PassFactory. I.e. if the pass is used in different
	// instances
	virtual std::string passName() { return passNameString; };

	// XXX move this to the ctor, as it should be read only
//	void setPassName(std::string passName) { passNameString = passName; };

	std::vector<Pass *> const getInputPasses() { return channelConfig.getPasses(); };

	Core::ConstructTraitType getMinInputLevelRequirement(Pass *pass) { return channelConfig.getMinConstructLevel(pass); };

	Core::ConstructTraitType getMaxInputLevelRequirement(Pass *pass) { return channelConfig.getMaxConstructLevel(pass); };

 protected:
	// Get the number of altered, invalidated or changed constructs. We expect the next higher construct that dominates
	// the altered or deleted constructs
	const Core::ConstructSet *getCollisionSet() { return passImplementation->cgetCollisionSet(); }

 private:
	// These flags are solely used to ensure proper sequences of initialization, execution and finalization
	bool passInitialized, passExecuted, passFinalized;

	const std::string passNameString; // XXX make const

	std::unordered_map<Pass *, std::shared_ptr<Core::ConstructSet> > inputOverride;

	/* Tells the pass which input passes it needs to know of */
	InstRO::Core::ChannelConfiguration channelConfig;

	// A Pointer to the compiler-specific implementation
	InstRO::Core::PassImplementation *passImplementation;
};
}	// InstRO
#endif
