#ifndef INSTRO_CORE_SIMPLE_PASSMANAGER_H
#define INSTRO_CORE_SIMPLE_PASSMANAGER_H

#include <vector>
#include <algorithm>

#include "instro/core/PassManager.h"

namespace InstRO {
namespace PassManagement {

/**
 * The SimplePassManager does not generate depence graphs.
 * It runs the registered passes in the order they have been registered.
 * First, it invokes the init() method on all passes in the order they have been registered.
 * Secondly, all passes are executed in the order they haven been registered.
 * Lastly, the finalize() method is called an all passes in the order they have been registered.
 */
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

	/** Executes the actual configuration of passes */
	int execute() override;

	/** In a simple pass manager this function should just be left blang? */
	virtual void setDependence(Pass *pred, Pass *pass) { throw std::string("Not implemented in SimplePassManager"); }

	virtual bool hasOutputDependencies(Pass *pass) {
		for (auto p : passList) {
			if ((p == pass) || (!hasInputDependencies(p))) {
				continue;
			}

			auto cVec(getPredecessors(p));
			if (std::find_if(cVec.begin(), cVec.end(), [pass](Pass *pp) { return pp == pass; }) != cVec.end()) {
				return true;
			}
		}

		return false;
	}

	virtual bool hasInputDependencies(Pass *pass) { return getPredecessors(pass).size() > 0; };

 protected:
	/** Retrieves the list of predecessors of pass p */
	const std::vector<Pass *> getPredecessors(Pass *p) const { return p->getInputPasses(); };

	// We own the passes
	std::vector<Pass *> passList;
};

}	// PassManagement
}	// InstRO
#endif
