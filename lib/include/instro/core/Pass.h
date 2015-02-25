#ifndef INSTRO_CORE_PASS_H
#define INSTRO_CORE_PASS_H
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


#include "instro/core/ConstructSetManagement.h"
#include "instro/core/ConstructLevelManagrment.h"

//#include "instro/roseInstRO.h"

namespace InstRO{
	/* PassFactory: Interface for the mandatory InstRO Passes. */
	namespace PassManagement
	{
		class PassManager;
	}
	
	class Pass:public ::InstRO::Core::PassConstructSetManagement, public ::InstRO::Core::ConstructLevelManagrment
	{
	public:
		Pass(){};

		// External Interface used by the PassManager
		void init(){};
		void enableInput(){};
		void disableInput(){};
		bool isInputEnabled(){};
		void enableOutput(){};
		void finalizeOutput(){};
		bool isOutputEnabled(){};
		void execute(){};
		void finalize(){};/*
			};
			template <class T|> class PassImpl:public Pass
			*/
	};
}

#endif