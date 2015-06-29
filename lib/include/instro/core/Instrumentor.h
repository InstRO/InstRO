#ifndef INSTRO_CORE_INSTRUMENTOR_H
#define INSTRO_CORE_INSTRUMENTOR_H
/*
 * This is our user-level interface. A typical use case in our minds is
 * int main(...){
 *   InstRO myinstro = ROSE::getInstRO();
 *   ROSE::InstROFactory fac = myinstro.getFactory();
 *   Pass *sel = fac.getSelectionPass(myinstro.getPassManager());
 *   Pass *adapter = fac.getAdapterPass(myinstro.getPassManager(), sel);
 *   myinstro.run();
 * }
 */

#include <list>
#include <map>
#include <string>
#include <iostream>
#include <vector>

#include "instro/core/ConstructLevelManagrment.h"
#include "instro/core/ConstructSetManagement.h"
#include "instro/core/ConstructSet.h"
#include "instro/core/PassManager.h"
#include "instro/core/SimplePassManager.h"
#include "instro/core/PassFactory.h"

#include "instro/tooling/analysisInterface.h"

namespace InstRO {
namespace PassManagement {
class PassManager;
}	// PassManagement

class Instrumentor {
 public:
	typedef enum CompilationPhase {
		firstPhase = 1,
		defaultPhase = 1,
		frontend,
		afterOptimization,
		afterAssebling,
		afterLinking,
		lastPhase
	} CompilationPhase;

 public:
	Instrumentor() {
		passManagerLocked = false;
		new InstRO::PassManagement::SimplePassManager();
		setPassManager(new InstRO::PassManagement::SimplePassManager());
	}
	virtual InstRO::PassFactory* getFactory(CompilationPhase phase = frontend) = 0;
	virtual InstRO::PassManagement::PassManager* getPassManager() { return passManager; }
	void setPassManager(InstRO::PassManagement::PassManager* manager) {
		if (passManagerLocked)
			//			throw std::string("PassManager already in use and locked");
			std::cerr << "PassManager already in use and locked" << std::endl;
		else {
			passManager = manager;
		}
	}
	Tooling::AnalysisManager * getAbstractLayer(){
		if (analysisManager != NULL)
			return analysisManager;
		else {
			analysisManager = new Tooling::AnalysisManager();
			return analysisManager;
		}
	}
	//AbstractLayer::
 protected:
	bool passManagerLocked;
	InstRO::PassManagement::PassManager* passManager;
	InstRO::Tooling::AnalysisManager * analysisManager;

 public:
	virtual void init() = 0;
	virtual void apply() = 0;
	virtual void finalize() = 0;
};
}

#endif
