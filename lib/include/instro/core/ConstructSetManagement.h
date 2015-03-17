#ifndef INSTRO_CORE_CONSTRUCTSETMANAGEMENT_H
#define INSTRO_CORE_CONSTRUCTSETMANAGEMENT_H

#include <unordered_map>
#include <iostream>

#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Core {
class PassConstructSetManagement {
 public:
	PassConstructSetManagement() { output = NULL; };
	void setOutput(ConstructSet* output);
	ConstructSet* getOutput() { return output; };
	ConstructSet* getInput(PassConstructSetManagement* from);
	void overrideInput(PassConstructSetManagement* from,
										 ConstructSet* overrideSet);

 private:
	std::unordered_map<PassConstructSetManagement*, ConstructSet*> inputOverride;
	ConstructSet* output;

 public:
	// protected:
	// Internal Interface used by the derived passes
	//		ConstructSet* registerInput(int maxConstructLevel,std::string name);
	// ConstructSet* registerInput(int maxConstructLevel);
	//		ConstructSetContainer * getInputConstructSets();
	//		ConstructSet* registerOutput(int minConstructLevel);
	//	private:
	//		std::vector<ConstructSet*> inputSets;
	//		std::vector<ConstructSet*> outputSets;
};
}
}
#endif
