//#include "instro.h"
#include <vector>
#include <hash_map>

namespace InstRO{
class ConstructSet
{
public:
	int getConstructLevel(){};
	int getMaxConstructLevel(){};
	int getMinConstructLevel(){};
	ConstructSet(int level){};
	ConstructSet(int minLevel,int maxLevel){};
	int setCurrentMinLevel(){};
	int setCurrentMaxLevel(){};
};
typedef std::vector<ConstructSet*> ConstructSetContainer;

class PassConstructSetManagement
{
public:
		ConstructSet* getOutput();
	//
	void setInput(std::vector<ConstructSet*> input);
protected:
	// Internal Interface used by the derived passes	
	ConstructSet* registerInput(int maxConstructLevel,std::string name);
	ConstructSet* registerInput(int maxConstructLevel);
	ConstructSetContainer * getInputConstructSets();
	ConstructSet* registerOutput(int minConstructLevel);
private:
	std::vector<ConstructSet*> inputSets;
	std::vector<ConstructSet*> outputSets;
};


const int LiteralLevel=1;


class PassConstructLevelManagement
{
public:
	int getConstructLevelSupport();
protected:
	PassConstructLevelManagement(int min,int max)
	{
	}
	int getMinSupportedConstructLevel(){return minSupportedConstructLevel;}
	int getMaxSupportedConstructLevel(){return maxSupportedConstructLevel;}
private:
	int minSupportedConstructLevel,maxSupportedConstructLevel;
};

class Pass:public PassConstructSetManagement, public PassConstructLevelManagement
{
public:
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
	ConstructSet* getInput(Pass* from)
	{
		if (inputOverride.find(from)==inputOverride.end())
		{
			return from->getOutput();
		}
		else
		{
			return inputOverride[from];
		}
		throw std::string("I should no be here. It is impossible!!!");
	}
	void overrideInput(Pass * from, ConstructSet* overrideSet)
	{
		inputOverride[from]=overrideSet;
	}
private:
	std::hash_map<Pass*,ConstructSet*> inputOverride;
};

class PassEnvelope{
	friend class PassManager;
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
	PassEnvelope * registerPass(Pass * currentPass)
	{
		PassEnvelope * newPass=new PassEnvelope(currentPass);
		passList.push_back(newPass);
		return newPass;
	}
	//TODO: FIX
	void addDependency(Pass * pass, Pass * dependency)
	{
		for (std::vector<PassEnvelope *>::iterator i:passList)
		{
			if ((*i)->pass==pass)
			{
				// check if the inputlevel is ok
				//dependency->g
				(*i)->predecessors.push_back(dependency);
				return;
			}
		}
		// this pass is not registere
		registerPass(pass);
	//	addDependency(pass,dependency,requirement);
		//std::vector<Pass*>::iterator it=std::find(passList.begin(),passList.end(),pass);
		//if (it!=passList.end())
		//{
//			(*it)->pass->predecessors.push_back(dependency);
	};
	int execute()
	{
		for (PassEnvelope * passContainer: passList)
		{
			// Allow the Pass to Initialize iself. E.g. start reading input data from files, allocated named input fields, etc.
			// passContainer->pass->init();
		}
		for (PassEnvelope * passContainer: passList)
		{
				std::vector<ConstructSet*> constructSets;
				// Collect Output from preceeding Passes
				for (Pass * predPass:passContainer->predecessors)
				{
					// TODO: Check if InputLevels match
					constructSets.push_back(predPass->getOutput());
				}
				
			//	passContainer->pass->setInput(constructSets);
				passContainer->pass->enableInput();
				passContainer->pass->init();
				passContainer->pass->execute();
				passContainer->pass->finalize();
		}
		for (PassEnvelope * passContainer: passList)
		{
			passContainer->pass->disableInput();
		}
		for (PassEnvelope * passContainer: passList)
		{
			passContainer->pass->finalizeOutput();
		}
	};
protected:
	std::vector<PassEnvelope*> passList;
};

class PassFactory{
				public:	
					PassFactory(PassManager * refManager):refToGobalPassManager(refManager){};
				virtual Pass * createBlackNWhiteFilter(Pass * input);
				virtual Pass * createBlackNWhiteSelector(std::string string);
				virtual Pass * createBooleanOrSelector(Pass * inputA,Pass * inputB);
				virtual Pass * createProgramEntrySelector();
				virtual Pass * createCygProfileAdapter(Pass * input);
protected:
			PassManager * refToGobalPassManager;
};

class InstRO
{
public:
	typedef enum CompilationPhase
	{
		firstPhase,
		defaultPhase,
		frontend,
		afterOptimization,
		afterAssebling,
		afterLinkin,
		lastPhase
	}CompilationPhase;
public:
		
		virtual PassFactory * getFactory(CompilationPhase phase=frontend)=0;
protected:
	PassManager * passManager;
	virtual init()=0;
	virtual execute()=0;
};

	
}

namespace InstRO
{
	namespace Rose{
		class RosePass: public Pass
		{};
		class Selector:public RosePass
		{
		};
		class Adapter:public RosePass
		{
		};
		class Transformer: public RosePass
		{
		};
			class RosePassFactory:public PassFactory
			{
			public:	
				RosePassFactory(PassManager * refManager):PassFactory(refManager){};
				RosePass * createBlackNWhiteFilter(RosePass * input)
				{
					RosePass * pass=new RosePass(input);
					refToGobalPassManager->registerPass(pass);
					
//					refToGobalPassManager->addDependency(pass,input,1);
					//,std::vector<Pass*>().push_back(input));
				}
				RosePass * createBlackNWhiteSelector(std::string string);
				RosePass * createBooleanOrSelector(RosePass * inputA,RosePass * inputB);
				RosePass * createProgramEntrySelector();
				RosePass * createCygProfileAdapter(RosePass * input);
			};
		}; // Rose Name Space

	class RoseInstrumentor: public InstRO
	{
	public: 
		::InstRO::Rose::RosePassFactory * getFactory(CompilationPhase phase){return new ::InstRO::Rose::RosePassFactory(passManager);}
	};
}


//using namespace InstRO;

int main(int argc,char ** argv)
{
	::InstRO::InstRO * instro=new ::InstRO::RoseInstrumentor();
	::InstRO::PassFactory * factory = instro->getFactory();
	::InstRO::Pass * bnwSelector,* programEntrySelector,*orSelector,*adapter;
	bnwSelector=factory->createBlackNWhiteSelector("HelloWorld*");
	programEntrySelector=factory->createProgramEntrySelector();
	orSelector=factory->createBooleanOrSelector(bnwSelector,programEntrySelector);
	adapter=factory->createCygProfileAdapter(orSelector);
	instro->init();
	::InstRO::Rose::Selector * myDevelopStuff=new ::InstRO::Rose::Selector();
	instro->getPassManager()->registerPass
	instro->apply();
	instro->finalize();


/*	::InstRO::InstRO * instro;
	//CI: Rose specific constructor for the RoseInstrumentor - factory
	instro=new ::InstRO::RoseInstrumentor(&argc,&argv);
	::InstRO::Factory * fac=instro->getFactory();
	::InstRO::Selector * exampleSelector=fac->createExampleSelector();
	instro->printDebugStatus();

	Pass *PickerA,*PickerB,*Filter;
	*/

	return 0;
}