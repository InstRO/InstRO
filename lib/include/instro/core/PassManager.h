#ifndef INSTRO_CORE_PASSMANAGER_H
#define INSTRO_CORE_PASSMANAGER_H

#include <vector>
#include "instro/core/Pass.h"

namespace InstRO {
namespace Core {
class ConstructSet;
}

namespace PassManagement {

class PassManager {
 protected:
	// Since the PassManager is an Interface and its children will not be able to access the collisionSet, the following
	const InstRO::Core::ConstructSet *getCollisionSet(InstRO::Pass *pass) { return pass->getCollisionSet(); }

 public:
	friend Pass;

	virtual ~PassManager() {}

	/** A Pass must be registered to the PassManager when it is constructed.
	 * The PassManager can implement this method to do whatever wiring is necessary in its specific implementation.
	 */
	virtual void registerPass(Pass *currentPass) = 0;
	
	/** Runs the configuration using whatever features the PassManager implementation provides */
	virtual int execute() = 0;

	virtual bool hasOutputDependencies(Pass *) = 0;

	virtual bool hasInputDependencies(Pass *) = 0;

	// To allow explicit sequencing the user can use this call to establish an explicit before-after relationship between
	// passes.
	// This sequencing is overruled, by the input-dependencies.
	virtual void setDependence(Pass *predecessor, Pass *pass) = 0;
};

}	// End namespace PassManagement
}	// End namespace InstRO

#endif
