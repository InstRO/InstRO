#ifndef INSTRO_CORE_PASSMANAGER_H
#define INSTRO_CORE_PASSMANAGER_H

#include <vector>

// #include "instro.h"
//#include "instro/interface.h"
//#include "instro/core.h"
// #include "instro/pass_management.h"
#include "instro/core/Pass.h"

namespace InstRO{
	using namespace Core;
	class Pass;
	namespace Core{
		class ConstructSet;
	}
	namespace PassManagement{

class PassEnvelope
{
	friend class PassManager;
	friend class SimplePassManager;
	public:
		PassEnvelope(Pass * newPass)
		{
			pass=newPass;
			//			successor=NULL;
		}
	protected:
		Pass * pass;
		//Pass * successor;
		std::vector<Pass*> predecessors;
	};

class PassManager
	{
	public:
		// Enable the Pass Manager to query the pass for its dependencies
		virtual void registerPass(Pass * currentPass)=0;
		virtual int execute()=0;
protected:

};

class SimplePassManager: public InstRO::PassManagement::PassManager
{
		// Enable the Pass Manager to query the pass for its dependencies
		void registerPass(Pass * currentPass)
		{
			// CI: Create a new pass envelope to store the dependencies of this pass.
			PassEnvelope * newPass=new PassEnvelope(currentPass);
			passList.push_back(newPass);
		//	newPass->predecessors=currentPass->getInputPasses();
			// for all predecessors inquired the pass dependencies
			for (auto & i:newPass->predecessors)
			{
				Core::ContstructLevelType maxInputLevel=currentPass->getInputLevelRequirement(i);
				Core::ContstructLevelType minOutputLevelProvided=i->getOutputLevel();
				if (minOutputLevelProvided>maxInputLevel) throw std::string("InputPass ")+i->passName()+std::string(" can not provide ConstructLevel \"")+contstructLevelToString(maxInputLevel)+std::string("\"");
				addDependency(i,minOutputLevelProvided,currentPass,maxInputLevel);
			}
		}

	//TODO: FIX
	void addDependency(Pass * input,Core::ContstructLevelType inputLevel, Pass * currentPass,Core::ContstructLevelType requiredInputLevel)
	{
	// TODO(CI): implement storing of construct level graph
		getEnvelope(currentPass)->predecessors.push_back(input);
	};
	int execute()
	{
		for (PassEnvelope * passContainer: passList)
		{
			// Allow the Pass to Initialize iself. E.g. start reading input data from files, allocated named input fields, etc.
			// passContainer->pass->init();
		}
		for (PassEnvelope* passEnvelope: passList)
		{
			std::vector<ConstructSet*> tempConstructSets(getPredecessors(passEnvelope).size());
			//check if some input needs to be explicitly elevated
			for (auto &i: getPredecessors(passEnvelope))
			{
				if (i->getOutputLevel()<passEnvelope->pass->getInputLevelRequirement(i))
				{
					ConstructSet * newConstructSet=elevate(passEnvelope->pass->getInputLevelRequirement(i));
					passEnvelope->pass->overrideInput(i,newConstructSet);
					tempConstructSets.push_back(newConstructSet);

				}
			}
			// 1rst enable the input for the current pass. Since this is the basic pass manager, the sequence is linear with no intelligence. Hence, all preceeding passes must have already completed.
			passEnvelope->pass->enableInput();

			// 2nd: Allow the pass to initlialize its internal state, such that it can execute. This allows, e.g. to allocate large amounts of memory. Or preprocessing input. or whatever. However, modification or selection is not allowed.
			passEnvelope->pass->init();

			// 3rd: Execute the pass
			passEnvelope->pass->execute();

			// 4th: Tell the pass to finalize. It is supposed to release memory, close files, etc. However, the output set must be maintained unitl disableOutput is called
			passEnvelope->pass->finalize();
			
			// 5th: Disable input and deallocated potentially create construct sets
			passEnvelope->pass->disableInput();
			for (auto &i:tempConstructSets) delete i;

		}
		for (PassEnvelope * passContainer: passList)
		{
			// disable output for all passes. This allows to release the output construct set
			passContainer->pass->finalizeOutput();
		}
		return 0;
	};
protected:
	ConstructSet * elevate(Core::ContstructLevelType inputLevel){
		// TODO(CI): Implement Elevation
		return new ConstructSet();
	}
	bool isElevationRequired(){return false;};
	PassEnvelope * getEnvelope(Pass * pass)
	{
		for (auto &i:passList)
		{
			if (i->pass==pass) return i;
		}
		return NULL;
/*		std::vector<PassEnvelope*>::iterator result=std::find(passList.begin(),passList.end(),pass);
		if (result==passList.end()) return NULL;
		else return (*result);*/
	};
	std::vector<Pass*> getPredecessors(PassEnvelope * envelope){return envelope->predecessors;};

	std::vector<PassEnvelope*> passList;

};


	} // End namespace PassManagement
} // End namespace InstRO

#endif