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
		for (Pass *p : passList) {
			delete p;
		}
	}

	// Enable the Pass Manager to query the pass for its dependencies
	void registerPass(Pass *currentPass) override;

	int execute() override;

	virtual void setDependence(Pass *pred, Pass *pass) {
		// FIXME what is the actual semantic of an explicit dependence?
	}

	virtual bool hasOutputDependencies(Pass *pass) { return true; }
	virtual bool hasInputDependencies(Pass *pass) { return getPredecessors(pass).size() > 0; };

 protected:
	// Flatten the configuration graph to a sequence preserving input-ouput order
	bool createPassTraversalOder();

	std::vector<Pass *> getPredecessors(Pass *p) { return p->getInputPasses(); };

	// We own the passes
	std::vector<Pass *> passList;
};
}	// PassManagement
}	// InstRO
#endif
