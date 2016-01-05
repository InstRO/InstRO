#ifndef INSTRO_CORE_SIMPLE_PASSMANAGER_H
#define INSTRO_CORE_SIMPLE_PASSMANAGER_H

#include <vector>

#include "instro/core/PassManager.h"

namespace InstRO {
namespace PassManagement {

class SimplePassManager : public InstRO::PassManagement::PassManager {
 public:
	SimplePassManager(){};

	virtual ~SimplePassManager() {
		for (PassEnvelope *pe : passList) {
			delete pe;
		}
	}

	// Enable the Pass Manager to query the pass for its dependencies
	void registerPass(Pass *currentPass) override;

	void addDependency(Pass *input, Pass *currentPass) {
		// TODO(CI): implement storing of construct level graph
		getEnvelope(currentPass)->predecessors.push_back(input);
		getEnvelope(input)->existingOuputDependency = true;
	};

	int execute() override;

	virtual bool hasOutputDependencies(Pass *pass) { return getEnvelope(pass)->existingOuputDependency; };
	virtual bool hasInputDependencies(Pass *pass) { return getPredecessors(getEnvelope(pass)).size() > 0; };

	virtual void setDependence(Pass * predecessor, Pass * pass) {
		// TODO CI valudate functionality
		addDependency(predecessor,pass);		
	}

 protected:
	// Flatten the configuration graph to a sequence preserving input-ouput order
	bool createPassTraversalOder();
 
	PassEnvelope *getEnvelope(Pass *pass) {
		for (auto &i : passList) {
			if (i->pass == pass)
				return i;
		}
		return NULL;
	};
	std::vector<Pass *> getPredecessors(PassEnvelope *envelope) { return envelope->predecessors; };
	
	Pass *getPass(PassEnvelope *env) { return env->pass; };

	std::vector<PassEnvelope *> passList;
};
}	// PassManagement
}	// InstRO
#endif
