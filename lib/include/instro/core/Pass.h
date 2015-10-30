#ifndef INSTRO_CORE_PASS_H
#define INSTRO_CORE_PASS_H

#include <map>
#include <string>
#include <iostream>
#include <cassert>

#include <vector>
#include <unordered_map>

#include "instro/core/ConstructSet.h"
#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace PassManagement {
class PassManager;
}

/*
 * The Pass class is the administration class used within the pass manager.
 * It holds information necessary in order to singalize the pass implementation
 * whether it can be run or not.
 * Also it exposes a clear interface to the PassManager
 */
class Pass {
	friend InstRO::PassManagement::PassManager;

 public:
	// CI empty pass construction disallowed. Each Pass is an container for the corresponding PassImplementation Object
	// from the ToolingSpace
	Pass() = delete;
	Pass(Core::PassImplementation *pImpl)
			: passInitialized(false), passExecuted(false), passFinalized(false), passImplementation(pImpl){};

	Core::PassImplementation *getPassImplementation() { return passImplementation; };
	virtual ~Pass() {
		delete (passImplementation);
		passImplementation = nullptr;
	}
	// CI: Tell the pass, that is is allowed to initialize itself
	void initPass();
	// CI: Execute the pass, this generates the output-constructset
	void executePass();
	void finalizePass();

	/*-------------------------------------------------------------*/
	/* management of passes: enumerate inputs, query output, etc   */
	/*-------------------------------------------------------------*/
	std::unordered_map<Pass *, std::shared_ptr<Core::ConstructSet> > inputOverride;

	// Query the proxy pass for its output
	Core::ConstructSet *getOutput() { return passImplementation->getOutput(); };

	Core::ConstructSet *getInput(Pass *from) {
		if (inputOverride.find(from) == inputOverride.end())
			return from->getOutput();
		else
			return inputOverride.find(from)->second.get();
	}

	void overrideInput(Pass *from, std::unique_ptr<Core::ConstructSet> overrideSet) {
		std::unique_ptr<Core::ConstructSet> ovrdSet = std::move(overrideSet);
		std::shared_ptr<Core::ConstructSet> sharedSet = std::move(ovrdSet);
		inputOverride[from] = sharedSet;
	}

	// This allows for passes to have a unique name defined by the PassFactory. I.e. if the pass is used in different
	// instances
	virtual std::string passName() { return passNameString; };
	void setPassName(std::string passName) { passNameString = passName; };

	std::vector<Pass *> const getInputPasses() { return passImplementation->getChannelConfig().getPasses(); };
	Core::ConstructTraitType getMinInputLevelRequirement(Pass *pass) {
		return passImplementation->getChannelConfig().getMinConstructLevel(pass);
	};
	Core::ConstructTraitType getMaxInputLevelRequirement(Pass *pass) {
		return passImplementation->getChannelConfig().getMaxConstructLevel(pass);
	};
	// level; }
 protected:
	// Get the number of altered, invalidated or changed constructs. We expect the next higher construct that dominates
	// the altered or deleted constructs
	// alternate name getInvalidationSet()
	const Core::ConstructSet *getCollisionSet() { return passImplementation->cgetCollisionSet(); }

 private:
	// These flags are solely used to ensure proper sequences of initialization, execution and finalization
	bool passInitialized, passExecuted, passFinalized;

	std::string passNameString;

	// A Pointer to the compiler-specific implementation
	InstRO::Core::PassImplementation *passImplementation;
};
//}// Core
}	// InstRO
#endif
