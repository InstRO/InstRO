#ifndef INSTRO_CORE_INSTRUMENTOR_H
#define INSTRO_CORE_INSTRUMENTOR_H

#include "instro/PassFactory.h"
#include "instro/core/ConstructSet.h"
#include "instro/core/PassManager.h"
#include "instro/core/SimplePassManager.h"
#include "instro/core/Singleton.h"
#include "instro/tooling/AnalysisInterface.h"
#include "instro/utility/Container.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace InstRO {
namespace PassManagement {
class PassManager;
}	// namespace PassManagement

/**
 * \brief Interface to use, when constructing a new InstRO based instrumentation tool.
 *
 * The interface defines the methods which need to be overriden by the compiler specific implementation.
 * Passes are added using the returned PassFactory.
 * As of today, the PassManager used invokes all passes in the order of creation gouped by their methods. That is
 * for each pass run init
 * for each pass run execute
 * for each pass run finalize
 */
class Instrumentor {
 public:
	/**
	 * Indicates at which point in the compilation process InstRO should apply.
	 */
	typedef enum CompilationPhase {
		firstPhase = 1,
		defaultPhase = 1,
		frontend = 1,
	} CompilationPhase;

	/**
	 * Constructs an Instrumentor instance using the supplied PassManager.
	 * If nothing is supplied it constructs an instance of SimplePassManager.
	 * The instrumentor takes ownership of the PassManager
	 */
	Instrumentor(InstRO::PassManagement::PassManager* pm = new InstRO::PassManagement::SimplePassManager())
			: passManager(pm) {}

	virtual ~Instrumentor() { delete passManager; }

	/**
	 * Applies the configuration
	 */
	virtual void apply() = 0;

	/**
	 * Get an instance of the PassFactory. The PassFactory is internally managed and deconstructed.
	 * This method must be overridden by a platform specific implementation to return that platforms factory
	 */
	virtual InstRO::PassFactory* getFactory(CompilationPhase phase = frontend) = 0;

	/**
	 * Get an instance of the PassManager. The PassManager is internally managed and deconstructed.
	 */
	virtual const InstRO::PassManagement::PassManager* getPassManager() const { return passManager; }

	/**
	 * Interface to access the compiler specific implementation of the analysis layer.
	 */
	virtual Tooling::AnalysisManager* getAnalysisManager() = 0;

	/** Sets the default policy used whenever the pass manager encounters a necessary elevation */
	void setConstructRaisingPolicyCrop() { constructRaisingPolicyElevate = false; }
	/** Sets the default policy used whenever the pass manager encounters a necessary elevation */
	void setConstructRaisingPolicyElevate() { constructRaisingPolicyElevate = true; }
	/** Sets the default policy used whenever the pass manager encounters a necessary lowering */
	void setConstructLoweringPolicyCrop() { constructLoweringPolicyElevate = false; }
	/** Sets the default policy used whenever the pass manager encounters a necessary lowering */
	void setConstructLoweringPolicyElevate() { constructLoweringPolicyElevate = true; }

	bool getConstructRaisingPolicyCrop() const { return constructRaisingPolicyElevate; }
	bool getConstructRaisingPolicyElevate() const { return constructRaisingPolicyElevate; }
	bool getConstructLoweringPolicyCrop() const { return constructLoweringPolicyElevate; }
	bool getConstructLoweringPolicyElevate() const { return constructLoweringPolicyElevate; }

	/* Interface to register commandline handler objects, to add additional command line options */
	static InstRO::Utility::MultitypeMap& getCmdLineHandlerMap() { return cmdLineHandlerMap; }

 protected:
	InstRO::PassManagement::PassManager* passManager;

	/* Stores a map to register command line handler */
	static InstRO::Utility::MultitypeMap cmdLineHandlerMap;

	// True means elevate, False means crop
	bool constructRaisingPolicyElevate, constructLoweringPolicyElevate;
};
}	// namespace InstRO
#endif
