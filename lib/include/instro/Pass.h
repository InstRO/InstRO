#ifndef INSTRO_CORE_PASS_H
#define INSTRO_CORE_PASS_H

#include "instro/core/ConstructSet.h"
#include "instro/core/ChannelConfiguration.h"
#include "instro/core/PassImplementation.h"

#include <memory>
#include <string>
#include <cassert>

#include <vector>
#include <unordered_map>


namespace InstRO {
namespace PassManagement {
class PassManager;
}

/**
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

	/** Constructs a Pass, taking ownership of the PassImplementation and its configuration */
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

	// Finalizes the pass, ie close files etcpp
	void finalizePass();

	// Query the proxy pass for its output
	Core::ConstructSet *getOutput() const { return passImplementation->getOutput(); };

	// returns the ConstructSet for the input channel from
	Core::ConstructSet *getInput(Pass *from) {
		if (inputOverride.find(from) == inputOverride.end()) {
			return from->getOutput();
		} else {
			return inputOverride.find(from)->second.get();
		}
	}

	/** returns the ConstructSet for input channel */
	const Core::ConstructSet *getInput(int channel) const { return channelConfig.getConstructSetForChannel(channel); }

	// Allows to inject a ConstructSet which is returned for the pass with id from
	void overrideInput(Pass *from, std::unique_ptr<Core::ConstructSet> overrideSet) {
		std::shared_ptr<Core::ConstructSet> sharedSet(std::move(overrideSet));
		inputOverride[from] = sharedSet;
	}

	const std::string passName() const { return passNameString; };

	const std::vector<Pass *> getInputPasses() const { return channelConfig.getPasses(); };

	Core::ConstructTraitType getMinInputLevelRequirement(Pass *pass) const {
		return channelConfig.getMinConstructLevel(pass);
	};

	Core::ConstructTraitType getMaxInputLevelRequirement(Pass *pass) const {
		return channelConfig.getMaxConstructLevel(pass);
	};

 protected:
	// Get the number of altered, invalidated or changed constructs. We expect the next higher construct that dominates
	// the altered or deleted constructs
	const Core::ConstructSet *getCollisionSet() const { return passImplementation->getCollisionSet(); }

 private:
	// These flags are solely used to ensure proper sequences of initialization, execution and finalization
	bool passInitialized, passExecuted, passFinalized;

	const std::string passNameString;

	std::unordered_map<Pass *, std::shared_ptr<Core::ConstructSet> > inputOverride;

	// Tells the pass which input passes it needs to know of
	InstRO::Core::ChannelConfiguration channelConfig;

	// A Pointer to the compiler-specific implementation
	InstRO::Core::PassImplementation *passImplementation;
};
}	// InstRO
#endif
