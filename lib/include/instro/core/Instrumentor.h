#ifndef INSTRO_CORE_INSTRUMENTOR_H
#define INSTRO_CORE_INSTRUMENTOR_H

#include <map>
#include <string>
#include <iostream>
#include <vector>

#include "instro/core/Singleton.h"
#include "instro/core/ConstructSet.h"
#include "instro/core/PassManager.h"
#include "instro/core/SimplePassManager.h"
#include "instro/core/PassFactory.h"
#include "instro/tooling/AnalysisInterface.h"

namespace InstRO {
namespace PassManagement {
class PassManager;
}	// PassManagement

class Instrumentor {
 public:
	typedef enum CompilationPhase {
		firstPhase = 1,
		defaultPhase = 1,
		frontend = 1,
		// CI: This may be possible at a later point, e.g. in CLANG, but it currently does not apply
		//		afterOptimization,
		//		afterAssebling,
		//		afterLinking,
		//		lastPhase
	} CompilationPhase;

	Instrumentor() {
		passManagerLocked = false;
		setPassManager(new InstRO::PassManagement::SimplePassManager());
	}

	virtual ~Instrumentor() { delete passManager; }
	
	virtual void init() = 0;
	virtual void apply() = 0;
	virtual void finalize() = 0;

	// Get a instance of the PassFactory. The PassFactory is internally managed and deconstructed.
	// This method must be overridden by a platform specific implementation to return that platforms factory
	virtual InstRO::PassFactory* getFactory(CompilationPhase phase = frontend) = 0;

	// Get a instance of the PassManager. The PassManager is internally managed and deconstructed.
	virtual InstRO::PassManagement::PassManager* getPassManager() { return passManager; }

 public:
	// We allow to replace the passmangager with a different version or implementation.
	// This can only be done before the first pass is created from the factory, as the manager tracks all
	// passes, dependencies and invoces the passes accordingly
	void setPassManager(InstRO::PassManagement::PassManager* manager) {
		if (passManagerLocked) {
			std::cerr << "PassManager already in use and locked" << std::endl;
		} else {
			passManager = manager;
		}
	}

 public:
	void setConstructRaisingPolicyCrop() { constructRaisingPolicyElevate = false; };
	void setConstructRaisingPolicyElevate() { constructRaisingPolicyElevate = true; }
	void setConstructLoweringPolicyCrop() { constructLoweringPolicyElevate = false; }
	void setConstructLoweringPolicyElevate() { constructLoweringPolicyElevate = true; }

	bool getConstructRaisingPolicyCrop() { return constructRaisingPolicyElevate; }
	bool getConstructRaisingPolicyElevate() { return constructRaisingPolicyElevate; }
	bool getConstructLoweringPolicyCrop() { return constructLoweringPolicyElevate; }
	bool getConstructLoweringPolicyElevate() { return constructLoweringPolicyElevate; }

	// Interface to access the implementation specific Analysis Layer Container
	virtual Tooling::AnalysisManager* getAnalysisManager() = 0;

 protected:
	InstRO::PassManagement::PassManager* passManager;
	InstRO::Tooling::AnalysisManager* analysisManager;

	bool passManagerLocked;

	bool constructRaisingPolicyElevate, constructLoweringPolicyElevate;

};
}
#endif
