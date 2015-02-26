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
/*
 * For reasoning why not to use see ConstructSetManagement.h
 */
//#include <hash_map>
#include <unordered_map>


#include "instro/core/ConstructSet.h"
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
		virtual std::string passName(){return std::string("PassInterface");};
		Pass(){};

		// External Interface used by the PassManager
		void init(){};
		void enableInput(){};
		void disableInput(){};
		bool isInputEnabled(){};
		void enableOutput(){};
		void disableOutput(){};
		void finalizeOutput(){};
		bool isOutputEnabled(){};
		void execute(){};
		void finalize(){};
	/* Interface for managing construct levels of input passes*/
	public:
		void setOutputLevel(Core::ContstructLevelType level){outputLevel=level;};
		Core::ContstructLevelType getOutputLevel(){return outputLevel;};
		void registerInputPass(Pass * pass ,Core::ContstructLevelType level){inputPasses.push_back(pass);setInputLevelRequirement(pass,level);}
		std::vector<Pass*> getInputPasses(){return inputPasses;};
		void setInputLevelRequirement(Pass * pass,Core::ContstructLevelType level){inputRequiredLevels[pass]=level;}
		Core::ContstructLevelType getInputLevelRequirement(Pass *pass){return inputRequiredLevels[pass];};
		std::vector<Pass*> inputPasses;
		Core::ContstructLevelType outputLevel;
		std::unordered_map<Pass*,Core::ContstructLevelType> inputRequiredLevels;
		/*
			};
			template <class T|> class PassImpl:public Pass
			*/
	};
}

#endif
