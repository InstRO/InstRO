#ifndef INSTRO_CORE_PASSMANAGER_H
#define INSTRO_CORE_PASSMANAGER_H

#include <vector>

// #include "instro.h"
//#include "instro/interface.h"
//#include "instro/core.h"
// #include "instro/pass_management.h"
#include "instro/core/Pass.h"

namespace InstRO {
class Pass;
namespace Core {
class ConstructSet;
}
namespace PassManagement {

class PassEnvelope {
	friend class PassManager;

 public:
	PassEnvelope(Pass* newPass) {
		pass = newPass;
		//			successor=NULL;
	}

 protected:
	Pass* pass;
	// Pass * successor;
	std::vector<Pass*> predecessors;
};

class PassManager {
 public:
	// Enable the Pass Manager to query the pass for its dependencies
	PassEnvelope* registerPass(Pass* currentPass) {
		PassEnvelope* newPass = new PassEnvelope(currentPass);
		passList.push_back(newPass);
		return newPass;
	}
	// TODO: FIX
	void addDependency(Pass* pass, Pass* dependency){
			/*
			for (std::vector<PassEnvelope*>::iterator i:passList)
			{
				if ((*i)->pass==pass)
				{
					// check if the inputlevel is ok
					//dependency->g
					(*i)->predecessors.push_back(dependency);
					return;
				}
			}*/
			// this pass is not registere
			// registerPass(pass);
			//	addDependency(pass,dependency,requirement);
			// std::vector<Pass*>::iterator it=std::find(passList.begin(),passList.end(),pass);
			// if (it!=passList.end())
			//{
			//			(*it)->pass->predecessors.push_back(dependency);
	};
	int execute() {
		for (PassEnvelope* passContainer : passList) {
			// Allow the Pass to Initialize iself. E.g. start reading input data from files, allocated named input fields,
			// etc.
			// passContainer->pass->init();
		}
		for (PassEnvelope* passContainer : passList) {
			//			PassEnvelope * 	passContainer=(*it);
			std::vector<Core::ConstructSet*> constructSets;
			// Collect Output from preceeding Passes
			for (Pass* predPass : passContainer->predecessors) {
				// TODO: Check if InputLevels match
				constructSets.push_back(predPass->getOutput());
			}

			//	passContainer->pass->setInput(constructSets);
			passContainer->pass->enableInput();
			passContainer->pass->init();
			passContainer->pass->execute();
			passContainer->pass->finalize();
		}
		for (PassEnvelope* passContainer : passList) {
			passContainer->pass->disableInput();
		}
		for (PassEnvelope* passContainer : passList) {
			passContainer->pass->finalizeOutput();
		}
	};

 protected:
	std::vector<PassEnvelope*> passList;
};

}	// End namespace PassManagement
}	// End namespace InstRO

#endif