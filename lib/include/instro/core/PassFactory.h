#ifndef INSTRO_CORE_PASSFACTORY_H
#define INSTRO_CORE_PASSFACTORY_H
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

#include "instro/core/Pass.h"
//  CI: There is a reference to the ClangPassExecuturer in the InstRO Core Layer.
// TODO-JP:  Please put this somewhere more suitable, e.g. in the clang section
//  #include "instro/clang/core/ClangPassExecuter.h"

namespace InstRO {
class Pass;
namespace PassManagement {
class PassManager;
}

/* PassFactory: Interface for the mandatory InstRO Passes. */
class PassFactory {
 public:
	/* CI: A PassFactory must be initialized with the PassManager. */
	PassFactory(PassManagement::PassManager* manager) : passManager(manager){};


 protected:
	/*	void registerPass(Pass * pass)
		{
			refToGobalPassManager->registerPass(pass);
		}*/
	PassManagement::PassManager* passManager;
};

}	// InstRO

#endif
