#ifndef INSTRO_INSTRUMENTOR_H
#define INSTRO_INSTRUMENTOR_H
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
	namespace PassManagement
	{
		class PassManager;
	}

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

	class Instrumentor
	{
	public:
		typedef enum CompilationPhase
		{
			firstPhase=1,
			defaultPhase=1,
			frontend,
			afterOptimization,
			afterAssebling,
			afterLinking,
			lastPhase
		}CompilationPhase;
	public:		
		virtual PassFactory * getFactory(CompilationPhase phase=frontend)=0;
		PassManagement::PassManager * getPassManager()
		{
			return passManager;
		}
		void setPassManager(PassManagement::PassManager * manager)
		{
			if (passManagerLocked)
				throw std::string("PassManager already in use and locked");
			else
			{
				passManager=manager;
			}
		}
	protected:
		bool passManagerLocked;
		PassManagement::PassManager * passManager;
	public:
		virtual void init()=0;
		virtual void apply()=0;
		virtual void finalize()=0;
};

	class Pass:public ::InstRO::Core::PassConstructSetManagement, public ::InstRO::Core::PassConstructLevelManagement
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

#ifndef INSTRO_INSTRUMENTOR_H
#define INSTRO_INSTRUMENTOR_H
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
namespace InstRO {
	
	class Pass {
		public:
			/** What exactly is our public interface? */
			virtual void run();
	};

	class PassFactory {
		public:
			virtual Pass * createFunctionDefinitionSelector() = 0;
			virtual Pass * createCygProfileAdapter(Pass *inputSelector) = 0;
	};

	class InstRO {
		public:
			PassFactory * getFactory() = 0;
		protected:
			PassManager * pm;
	};

}
#endif
