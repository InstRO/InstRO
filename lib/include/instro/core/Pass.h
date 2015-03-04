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
class PassImplementation
{
public:
	virtual void initPass()=0;
	virtual void executePass()=0;
	virtual void finalizePass()=0;
	virtual void releaseOutput()=0;
	virtual ConstructSet * getOuput(PassImplementation * pass)=0;
};

class Pass:public ::InstRO::Core::PassConstructSetManagement, public ::InstRO::Core::ConstructLevelManagrment
{
	public:
		Pass(PassImplementation * pImpl):passImplementation(pImpl),passInitialized(false),passExecuted(false),passFinalize(false),passOutputReleased(false){};
		PassImplementation * getPassImplementation(){return passImplementation;};
		void initPass()
		{	
			if (inputReady)
				passImplementation->initPass();
			else throw std::string("Input not ready!");
			passInitialized=true;
		}
		void executePass()
		{
			if (passInitialized)
				passImplementation->executePass();
			else
				throw std::string("Pass not Initialized!");
			passExecuted=true;
		}
		void finalizePass()
		{	
			if (passInitialized)
				passImplementation->finalizePass();
			else
				throw std::string("Must Initialize Pass First!");
			finalizePass=true;
		}
		void releaseOutput()
		{
			if (passExecuted)
				passImplementation->releaseOutput();
			else
				throw std::string("Pass not Executed, Execute Pass First!");
			passOutputReleased=true;

		}
	private:
		PassImplementation * passImplementation;
		bool passInitialized,passExecuted,passFinalize,passOutputReleased;
		bool inputReady;
		// CI: everything below is currently to be considered deprecated

		// Info Functions:
		bool providesOutput(){return passProvidesOutputFlag;}
		bool requiresInput(){return passRequiresInputFlag;}
	protected:
		void setProvidesOuput(){passProvidesOutputFlag=true;};
		void setRequiresInput(){passRequiresInputFlag=true;};
		void unsetProvidesOuput(){passProvidesOutputFlag=false;};
		void unsetRequiresInput(){passRequiresInputFlag=false;};
	private:
		bool passRequiresInputFlag,passProvidesOutputFlag;

	public:
		virtual std::string className(){return std::string("Pass");};
		virtual std::string passName(){return passNameString;};
		void setPassName(std::string passNAME){passNameString=passNAME;};
	private:
		std::string passNameString;
	public:
		Pass():passRequiresInputFlag(true),passProvidesOutputFlag(true),passNameString("DefaultPassName"){};

		// External Interface used by the PassManager
		void init(){};
		void enableInput(){};
		void disableInput(){};
		bool isInputEnabled(){return true;};
		void enableOutput(){};
		void disableOutput(){};
		void finalizeOutput(){};
		bool isOutputEnabled(){return true;};
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
