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

	/** 
	 * Constructs an Instrumentor instance using the supplied PassManager.
	 * If nothing is supplied it constructs an instance of SimplePassManager.
	 * The instrumentor takes ownership of the PassManager
	 */
	Instrumentor(InstRO::PassManagement::PassManager* pm = new InstRO::PassManagement::SimplePassManager())
			: passManager(pm) {}

	virtual ~Instrumentor() { delete passManager; }
	
	virtual void apply() = 0;

	// Get a instance of the PassFactory. The PassFactory is internally managed and deconstructed.
	// This method must be overridden by a platform specific implementation to return that platforms factory
	virtual InstRO::PassFactory* getFactory(CompilationPhase phase = frontend) = 0;

	// Get a instance of the PassManager. The PassManager is internally managed and deconstructed.
	virtual const InstRO::PassManagement::PassManager* getPassManager() const { return passManager; }

	void setConstructRaisingPolicyCrop() { constructRaisingPolicyElevate = false; };
	void setConstructRaisingPolicyElevate() { constructRaisingPolicyElevate = true; }
	void setConstructLoweringPolicyCrop() { constructLoweringPolicyElevate = false; }
	void setConstructLoweringPolicyElevate() { constructLoweringPolicyElevate = true; }

	bool getConstructRaisingPolicyCrop() const { return constructRaisingPolicyElevate; }
	bool getConstructRaisingPolicyElevate() const { return constructRaisingPolicyElevate; }
	bool getConstructLoweringPolicyCrop() const { return constructLoweringPolicyElevate; }
	bool getConstructLoweringPolicyElevate() const { return constructLoweringPolicyElevate; }

	// Interface to access the implementation specific Analysis Layer Container
	virtual Tooling::AnalysisManager* getAnalysisManager() = 0;

 protected:
	InstRO::PassManagement::PassManager* passManager;
	InstRO::Tooling::AnalysisManager* analysisManager;

	bool constructRaisingPolicyElevate, constructLoweringPolicyElevate;

};
}
#endif
