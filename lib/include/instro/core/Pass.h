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
#include <cassert>

#include <vector>
#include <unordered_map>

#include "instro/core/ConstructSet.h"
#include "instro/core/PassExecuter.h"
#include "instro/core/PassImplementation.h"

// #include "instro/core/ConstructSetManagement.h"
// #include "instro/core/ConstructLevelManagrment.h"

//#include "instro/roseInstRO.h"
namespace InstRO {
//	namespace Core{
/* PassFactory: Interface for the mandatory InstRO Passes. */
namespace PassManagement {
class PassManager;
}



// Make it a sane world
class Pass{
 public:
	//CI empty pass construction disallowed. Each Pass is an container for the corresponding PassImplementation Object from the ToolingSpace
	Pass() = delete;
	Pass(Core::PassImplementation *pImpl) :
		passInitialized(false), passExecuted(false), passFinalized(false),
		passImplementation(pImpl){
	};

	Core::PassImplementation *getPassImplementation() { return passImplementation; };
	~Pass() {
		delete (passImplementation);
		passImplementation = NULL;
	}
	//CI: Tell the pass, that is is allowed to initialize itself
	void initPass();
	//CI: Execute the pass, this generates the output-constructset
	void executePass();
	//CI: Refactoring to remove the Executor stuff|	void execute(InstRO::PassManagement::PassExecuter *executer);
	void finalizePass();


	/*-------------------------------------------------------------*/
	/* management of passes: enumerate inputs, query output, etc   */
	/*-------------------------------------------------------------*/
	std::unordered_map<Pass*, Core::ConstructSet*> inputOverride;
	
	// Query the proxy pass for its output
	Core::ConstructSet* getOutput() {
		return passImplementation->getOutput();
	};

	Core::ConstructSet* getInput(Pass* from)
	{
		if (inputOverride.find(from) == inputOverride.end())
			return from->getOutput();
		else
			return inputOverride.find(from)->second;
	}

	void overrideInput(Pass* from,	Core::ConstructSet* overrideSet){
		inputOverride[from] = overrideSet;
	}

	

//2015-06-19 - Deprecated:	void releaseOutput();

	// CI: Enable Input is called externally to indicate, that  the input passes
	// have beend processed and that the current pass can now use the
	// getInput(Pass*) to obtain the construct sets of its predecessors
//	void setInputEnabled() { inputReady = true; };
	// CI: Disable Input is called externally to indicate, that the input passes
	// have cleared their internal state AND their output construct set such that
	// the
	//     getInput(Pass*) does not provide a vaild construct set. The provided
	//     construct set may no longer be used as it could have been deallocated
//	void setInputDisabled() { inputReady = false; };
	// CI: queryFunction to determin, if the input is enabled
//	bool isInputEnabled() { return true; };
//	bool isOutputEnabled() { return passExecuted && !passOutputReleased; }

	// Info Functions:
//	bool providesOutput() { return passProvidesOutputFlag; }
//	bool requiresInput() { return passRequiresInputFlag; }
//	void setProvidesOutput(bool value = true) { passProvidesOutputFlag = value; };
//	void setRequiresInput(bool value = true) { passRequiresInputFlag = value; };


// This allows for passes to have a unique name defined by the PassImplementation
	virtual std::string passName() { return passNameString; };
	void setPassName(std::string passName) { passNameString = passName; };

//	void setOutputLevel(Core::ConstructLevelType level) { outputLevel = level; };
//	Core::ConstructLevelType getOutputLevel() { return outputLevel; };

	//Deprecated|void registerInputPass(Pass *pass, Core::ConstructLevelType level) {		inputPasses.push_back(pass);		setInputLevelRequirement(pass, level);	}

	std::vector<Pass *> const getInputPasses() { return passImplementation->channelCFG().getPasses(); };
	Core::ConstructLevelType getMinInputLevelRequirement(Pass *pass) { return passImplementation->channelCFG().getMinConstructLevel(pass);	};
	Core::ConstructLevelType getMaxInputLevelRequirement(Pass *pass) { return passImplementation->channelCFG().getMaxConstructLevel(pass); };
	//Deprecated| void setInputLevelRequirement(Pass *pass, Core::ConstructLevelType level) { inputRequiredLevels[pass] = level; }



 private:
	 // These flags are solely used to ensure proper sequences of initialization, execution and finalization
	 bool passInitialized, passExecuted, passFinalized;


	// bool passRequiresInputFlag, passProvidesOutputFlag;
	// bool passInitialized, passExecuted, passFinalize, passOutputReleased;
	// bool inputReady;
	std::string passNameString;

	// A Pointer to the compiler-specific implementation
	Core::PassImplementation *passImplementation;

	// std::vector<Pass *> inputPasses;
//	Core::ConstructLevelType outputLevel;
//	std::unordered_map<Pass *, Core::ConstructLevelType> inputRequiredLevels;
};
//}// Core
}	// InstRO
#endif
