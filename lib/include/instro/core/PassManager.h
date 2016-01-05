#ifndef INSTRO_CORE_PASSMANAGER_H
#define INSTRO_CORE_PASSMANAGER_H

#include <vector>
#include "instro/core/Pass.h"

namespace InstRO {
namespace Core {
class ConstructSet;
}

namespace PassManagement {

class PassEnvelope {
	friend class SimplePassManager;

 public:
	PassEnvelope(Pass *newPass) {
		pass = newPass;
		existingOuputDependency = false;
	}

 protected:
	bool existingOuputDependency;
	Pass *pass;
	std::vector<Pass *> predecessors;
};

class PassManager {
 protected:
	// Since the PassManager is an Interface and its children will not be able to access the collisionSet, the following
	const InstRO::Core::ConstructSet *getCollisionSet(InstRO::Pass *pass) { return pass->getCollisionSet(); }

 public:
	friend Pass;

	virtual ~PassManager() { }

	// Enable the Pass Manager to query the pass for its dependencies
	virtual void registerPass(Pass *currentPass) = 0;
	virtual int execute() = 0;
	virtual bool hasOutputDependencies(Pass *) = 0;
	virtual bool hasInputDependencies(Pass *) = 0;

	// To allow explicit sequencing the user can use this call to establish an explicit before-after relationship between passes.
	// This sequencing is overruled, by the input-dependencies.
	virtual void setDependence(Pass * predecessor, Pass * pass) = 0;

};

}	// End namespace PassManagement
}	// End namespace InstRO

#endif
