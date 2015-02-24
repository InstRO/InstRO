#ifndef INSTRO_CORE_H
#define INSTRO_CORE_H
//#include "instro/interface.h"
#include <hash_map>

namespace InstRO{
	class Pass;
	namespace Core{
/* CI: The ConstructSet class is intended to be specialized for each compiler interface. It provides the basic mechanisms to specify what construct level are contained. */
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

//CI: consolidation of mechanisms to adminstrate construct sets contained within a pass
class PassConstructSetManagement
	{
	public:
		PassConstructSetManagement(){};
		ConstructSet* getInput(PassConstructSetManagement * from)
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
		void overrideInput(PassConstructSetManagement * from, ConstructSet* overrideSet)
		{
			inputOverride[from]=overrideSet;
		}
	private:
		std::hash_map<PassConstructSetManagement*,ConstructSet*> inputOverride;
	public:
		ConstructSet* getOutput(){return NULL;};
		//
		void setInput(std::vector<ConstructSet*> input){};
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

// Methods and routines to manage the requirements for interface
class PassConstructLevelManagement
	{
	public:
		PassConstructLevelManagement(){};
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

	} // End namespace Core
} // End namespace InstRO
#endif