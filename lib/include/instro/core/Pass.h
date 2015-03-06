#ifndef INSTRO_CORE_PASS_H
#define INSTRO_CORE_PASS_H
/*
 * This is our user-level interface. A typical use case in our minds is
 * int main(...){
 *   InstRO myinstro = ROSE::getInstRO();
 *   ROSE::InstROFactory fac = myinstro.getFactory();
 *   Pass *sel = fac.getSelectionPass(myinstro.getPassManager());
 *   Pass *adapter = fac.getAdapterPass(myinstro.getPassManager(), sel);
 *   myinstro.run();
 * }
 */

#include <list>
#include <map>
#include <string>
#include <iostream>
#include <vector>

#include <vector>
#include <unordered_map>

#include "instro/core/ConstructSet.h"
#include "instro/core/PassImplementation.h"

#include "instro/core/ConstructSetManagement.h"
#include "instro/core/ConstructLevelManagrment.h"

//#include "instro/roseInstRO.h"
namespace InstRO {
/* PassFactory: Interface for the mandatory InstRO Passes. */
namespace PassManagement {
class PassManager;
}

// Make it a sane world
class Pass : public ::InstRO::Core::PassConstructSetManagement,
						 public ::InstRO::Core::ConstructLevelManagrment {
 public:
	Pass() = delete;
	Pass(PassImplementation *pImpl)
			: passImplementation(pImpl),
				passInitialized(false),
				passExecuted(false),
				passFinalize(false),
				passOutputReleased(false),
				inputReady(false){};
	PassImplementation *getPassImplementation() { return passImplementation; };
	~Pass() {
		delete (passImplementation);
		passImplementation = NULL;
	}
	void initPass();
	void executePass();
	void finalizePass();
	void releaseOutput();
	// CI: Enable Input is called externally to indicate, that  the input passes
	// have beend processed and that the current pass can now use the
	// getInput(Pass*) to obtain the construct sets of its predecessors
	void setInputEnabled() { inputReady = true; };
	// CI: Disable Input is called externally to indicate, that the input passes
	// have cleared their internal state AND their output construct set such that
	// the
	//     getInput(Pass*) does not provide a vaild construct set. The provided
	//     construct set may no longer be used as it could have been deallocated
	void setInputDisabled() { inputReady = false; };
	// CI: queryFunction to determin, if the input is enabled
	bool isInputEnabled() { return true; };
	bool isOutputEnabled() { return passExecuted && !passOutputReleased; }

	// Info Functions:
	bool providesOutput() { return passProvidesOutputFlag; }
	bool requiresInput() { return passRequiresInputFlag; }
	void setProvidesOuput() { passProvidesOutputFlag = true; };
	void setRequiresInput() { passRequiresInputFlag = true; };
	void unsetProvidesOuput() { passProvidesOutputFlag = false; };
	void unsetRequiresInput() { passRequiresInputFlag = false; };

	virtual std::string passName() { return passNameString; };
	void setPassName(std::string passNAME) { passNameString = passNAME; };

 public:
	void setOutputLevel(Core::ContstructLevelType level) { outputLevel = level; };
	Core::ContstructLevelType getOutputLevel() { return outputLevel; };
	void registerInputPass(Pass *pass, Core::ContstructLevelType level) {
		inputPasses.push_back(pass);
		setInputLevelRequirement(pass, level);
	}
	std::vector<Pass *> getInputPasses() { return inputPasses; };
	void setInputLevelRequirement(Pass *pass, Core::ContstructLevelType level) {
		inputRequiredLevels[pass] = level;
	}
	Core::ContstructLevelType getInputLevelRequirement(Pass *pass) {
		return inputRequiredLevels[pass];
	};

 private:
	bool passRequiresInputFlag, passProvidesOutputFlag;
	PassImplementation *passImplementation;
	bool passInitialized, passExecuted, passFinalize, passOutputReleased;
	bool inputReady;
	std::string passNameString;

	std::vector<Pass *> inputPasses;
	Core::ContstructLevelType outputLevel;
	std::unordered_map<Pass *, Core::ContstructLevelType> inputRequiredLevels;
};
}

#endif
