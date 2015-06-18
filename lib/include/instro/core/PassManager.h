#ifndef INSTRO_CORE_PASSMANAGER_H
#define INSTRO_CORE_PASSMANAGER_H

#include <vector>
#include "instro/core/Pass.h"
#include "instro/core/PassExecuter.h"


namespace InstRO {
namespace Core {
class ConstructSet;
}

namespace PassManagement {

class PassEnvelope {
	//	friend class PassManager;
	friend class SimplePassManager;

 public:
	PassEnvelope(Pass *newPass) {
		pass = newPass;
		existingOuputDependency = false;
		//			successor=NULL;
	}

 protected:
	bool existingOuputDependency;
	//		bool existingInputDependency;
	Pass *pass;
	// Pass * successor;
	std::vector<Pass *> predecessors;
};

class PassManager {
 public:
	// Enable the Pass Manager to query the pass for its dependencies
	virtual void registerPass(Pass *currentPass) = 0;
	virtual void setExecuter(PassExecuter *executer) = 0;
	virtual int execute() = 0;
	//		virtual bool usesInput(Pass * pass){hasInputDependencies;
	// virtual bool isInput(Pass * pass)=0;
	virtual bool hasOutputDependencies(Pass *) = 0;
	virtual bool hasInputDependencies(Pass *) = 0;
 protected:
	// FIXME Why is that here?
	Pass *pass;
	// Pass * successor;
	// FIXME Why is that here?
	std::vector<Pass *> predeciessors;

	//2015-06-18 CI: JP - please explain to me why the exctutor has to be part of the pass manager ...
	// delegate for compiler specific invocation of a pass implementation
	PassExecuter *executer;
};

}	// End namespace PassManagement
}	// End namespace InstRO

#endif
