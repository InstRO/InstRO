#ifndef INSTRO_CORE_INSTRUMENTOR_H
#define INSTRO_CORE_INSTRUMENTOR_H

#include <map>
#include <string>
#include <iostream>
#include <vector>

// CI: deprecated | #include "instro/core/ConstructLevelManagement.h"
// CI: deprecated | #include "instro/core/ConstructSetManagement.h"

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

 public:
	Instrumentor() {
		passManagerLocked = false;
		setPassManager(new InstRO::PassManagement::SimplePassManager());
	}
	virtual ~Instrumentor() { delete passManager; }

	// Get a instance of the PassFactory. The PassFactory is internally managed and deconstructed.
	virtual InstRO::PassFactory* getFactory(CompilationPhase phase = frontend) = 0;
	// Get a instance of the PassManager. The PassManager is internally managed and deconstructed.
	virtual InstRO::PassManagement::PassManager* getPassManager() { return passManager; }

	void setPassManager(InstRO::PassManagement::PassManager* manager) {
		if (passManagerLocked)
			//			throw std::string("PassManager already in use and locked");
			std::cerr << "PassManager already in use and locked" << std::endl;
		else {
			passManager = manager;
		}
	}

 protected:
	bool constructRaisingPolicyElevate, constructLoweringPolicyElevate;

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
	bool passManagerLocked;
	InstRO::PassManagement::PassManager* passManager;
	InstRO::Tooling::AnalysisManager* analysisManager;

 public:
	virtual void init() = 0;
	virtual void apply() = 0;
	virtual void finalize() = 0;
};
}

#endif
