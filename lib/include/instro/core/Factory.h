#ifndef INSTRO_CORE_FACTORY_H
#define INSTRO_CORE_FACTORY_H
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

#include <vector>
#include <hash_map>


#include "instro/core.h"

//#include "instro/roseInstRO.h"

namespace InstRO{
	/* PassFactory: Interface for the mandatory InstRO Passes. */
	class PassFactory
	{
		public:	
			/* CI: A PassFactory must be initialized with the PassManager. */
			PassFactory(PassManagement::PassManager * refManager):refToGobalPassManager(refManager){};
			virtual Pass * createBlackNWhiteFilter(Pass * input)=0;
			virtual Pass * createBlackNWhiteSelector(std::string string)=0;
			virtual Pass * createBooleanOrSelector(Pass * inputA,Pass * inputB)=0;
			virtual Pass * createProgramEntrySelector()=0;
			virtual Pass * createCygProfileAdapter(Pass * input)=0;
		protected:
			PassManagement::PassManager * refToGobalPassManager;
	};
}
#endif