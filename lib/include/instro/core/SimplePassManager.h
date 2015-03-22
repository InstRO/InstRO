#ifndef INSTRO_CORE_SIMPLEPASSMANAGEMENT_H
#define INSTRO_CORE_SIMPLEPASSMANAGEMENT_H

#include <vector>

#include "instro/core/PassManager.h"

namespace InstRO {
namespace PassManagement {
class SimplePassManager : public InstRO::PassManagement::PassManager {
 public:
	 SimplePassManager(){};
	// Enable the Pass Manager to query the pass for its dependencies
	void registerPass(Pass *currentPass);
	void setExecuter(PassExecuter *executer);
	// TODO: FIX
	void addDependency(Pass *input, Core::ConstructLevelType inputLevel,
										 Pass *currentPass,
										 Core::ConstructLevelType requiredInputLevel) {
		// TODO(CI): implement storing of construct level graph
		getEnvelope(currentPass)->predecessors.push_back(input);
		getEnvelope(input)->existingOuputDependency = true;
	};
	int execute();

	virtual bool hasOutputDependencies(Pass *pass) {
		return getEnvelope(pass)->existingOuputDependency;
	};
	virtual bool hasInputDependencies(Pass *pass) {
		return getPredecessors(getEnvelope(pass)).size() > 0;
	};

 protected:
	InstRO::Core::ConstructSet *elevate(Core::ConstructLevelType inputLevel) {
		// TODO(CI): Implement Elevation
		return NULL;
	}
	bool isElevationRequired() { return false; };
	PassEnvelope *getEnvelope(Pass *pass) {
		for (auto &i : passList) {
			if (i->pass == pass) return i;
		}
		return NULL;
		/*		std::vector<PassEnvelope*>::iterator
			 result=std::find(passList.begin(),passList.end(),pass);
				if (result==passList.end()) return NULL;
				else return (*result);*/
	};
	std::vector<Pass *> getPredecessors(PassEnvelope *envelope) {
		return envelope->predecessors;
	};
	Pass *getPass(PassEnvelope *env) { return env->pass; };

	typedef std::vector<PassEnvelope *> PassEnvelopeListType;

	std::vector<PassEnvelope *> passList;
};
}	// PassManagement
}	// InstRO
#endif
