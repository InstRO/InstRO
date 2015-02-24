#ifndef INSTRO_INTERFACE_H
#define INSTRO_INTERFACE_H 0.1
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <vector>

using std::vector;
namespace InstRO{
class Pass;
class Selector;
class Transformer;
class Adapter;


class Factory
{
public:
	//virtual Selector * createSetSelector()=0;
	virtual Selector * createExampleSelector()=0;
};



class PassSequencer
{
public:
	virtual void addPass(Pass * pass)=0;
	virtual void triggerPasses()=0;
};
class DefaultPassSequencer:public PassSequencer
{
public:

protected:
	std::list<Pass*> passes;
};
class AutoSortSequencer:public PassSequencer
{
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
	InstRO():sequencerLocked(false){
	}
	virtual Factory * getFactory()=0;
	virtual std::list<CompilationPhase> getSupportedPhases(){return std::list<CompilationPhase>(1);};
	virtual void addPass(Pass * pass,CompilationPhase phase){
		if (!isValidCompilationPhase(phase)) throw (std::string("Invalid Phase")+phaseToString(phase));
		sequencerLocked=true;
	};
	void addPass(Pass * pass)
	{
		addPass(pass,InstRO::defaultPhase);
	}
	virtual void printDebugStatus()
	{
		std::cout << "sequencerLocked";
		if (sequencerLocked) std::cout <<" is set" << std::endl;
		else std::cout <<" is unset" << std::endl;
	}

protected:
	virtual std::string phaseToString(CompilationPhase phase)
	{
			return std::string("Unknown");
	}
	virtual bool isValidCompilationPhase(CompilationPhase phase)
	{
		return (phase==::InstRO::InstRO::defaultPhase);
	}

	void setSequencer(PassSequencer * pass){
		setSequencer(pass,InstRO::defaultPhase);
	}
	void setSequencer(PassSequencer * seq,CompilationPhase phase)
	{
		if (sequencerLocked) throw std::string("Sequencer Locked: A Pass was already added");
		sequencerList[phase]=seq;
	}
	std::map<CompilationPhase,PassSequencer *> sequencerList;
private:
	bool sequencerLocked;

};


class ConstructSet
{
public:
	int getConstructLevel();

};
typedef std::vector<ConstructSet*> ConstructSetContainer;

class Pass
{
public:
	ConstructSet* registerInput(int maxConstructLevel,Pass *);
	ConstructSetContainer * getInputSets();
	ConstructSet* registerOutput(int minConstructLevel);
	std::vector<ConstructSet*> getInputConstructSets();
protected:
	
	std::vector<ConstructSet*> inputSets;
	std::vector<ConstructSet*> outputSets;
private:
};






namespace Selectors{
	class Selector:public Pass	{};

	/* FileSelector: selects all constructs in files matching the predicate*/
	class FileSelector:public Selector{};

	/* IncludeSelector: selects included files from a depth and up to a depth */
	class IncludeFileSelector: public Selector{};

	/* Gammer Based Selectors*/
	/* CGrammerSelector: selects constructs based on their specific grammer defintions, such as, Declarations, Defintion, FunctionDefintion */
	class CGrammerSelector: public Selector{};
	class CxxGrammerSelector: public Selector{};
	class FORTRANGrammerSelector: public Selector{};

	class ProgramEntrySelector: public Selector{};
	class OpenMPSelector: public Selector{};
	class GlobalScopeSelector: public Selector{};


	/* NamePredicateSelectorTemplate: */
	class NamePredicateSelectorTemplate:public Selector{};
	class PragmaPredicateSelectorTemplate:public Selector{};
	class PragmaSelectorTemplate: public Selector{};
	class NamedEntitySelectorTemplate: public Selector{};
	

	/* Analysis and Structure Based Selectors */
	class CallPathSelector: public Selector{};
	class CallDepthSelector: public Selector{};
	class CashesSelector: public Selector{};
	class UniqueCallpathSelector: public Selector{};
	class CubeEdgeBaseSelector: public Selector{};
}

namespace Transformers{
	class Transformer:public Pass{};
}
namespace Adapters{
	class Adapter:public Pass{};
}

}
#endif
