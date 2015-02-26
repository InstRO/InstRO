#ifndef INSTRO_CORE_CONSTRUCTSETMANAGEMENT_H
#define INSTRO_CORE_CONSTRUCTSETMANAGEMENT_H
#include "instro/core/ConstructSet.h"

/*
 * According to stackoverflow and a gcc error hash_map is not part of the
 * standardized containers. MSVC and gcc seem to be providing it as a kind
 * of extension.
 * However, since we are using c++11 anyways, we should use unordered_map
 * instead. This is in the standard and is an associative container.
 */
//#include <hash_map>
#include <unordered_map>
namespace InstRO
{
namespace Core
{
class PassConstructSetManagement
	{
	public:
		PassConstructSetManagement(){output=NULL;};		
		void setOutput(ConstructSet * output);
		ConstructSet* getOutput(){return new ConstructSet();};
		ConstructSet* getInput(PassConstructSetManagement * from);
		void overrideInput(PassConstructSetManagement * from, ConstructSet* overrideSet);

	private:
		std::unordered_map<PassConstructSetManagement*,ConstructSet*> inputOverride;
		ConstructSet * output;

	public:
		
	
	//protected:
		// Internal Interface used by the derived passes	
//		ConstructSet* registerInput(int maxConstructLevel,std::string name);
		//ConstructSet* registerInput(int maxConstructLevel);
//		ConstructSetContainer * getInputConstructSets();
//		ConstructSet* registerOutput(int minConstructLevel);
//	private:
//		std::vector<ConstructSet*> inputSets;
//		std::vector<ConstructSet*> outputSets;
	};

}
}
#endif
