#ifndef INSTRO_CORE_CONSTRUCTSET_H
#define INSTRO_CORE_CONSTRUCTSET_H 0.1

#include <string>
// XXX Why do we want to use list here? I would prefer using vector!
//#include <list>
#include <vector>

namespace InstRO {

namespace Core {

typedef enum ContstructLevelEnum {
	ConstructLevelMin = 1,
	ConstructLevelLiteral = 1,
	ConstructLevelExpression = 2,
	ConstructLevelStatement,
	MaxConstructLevel = 1
} ConstructLevelType;

/*
class Construct
{
};
*/

std::string contstructLevelToString(ConstructLevelType type);
std::string operator+(const std::string &lhs, const ConstructLevelType &type);


/* CI: The ConstructSet class is intended to be specialized for each compiler
 * interface. It provides the basic mechanisms to specify what construct level
 * are contained. 
 */
class ConstructSet {
 public:
	ConstructSet():minimalSupportedLevel(ConstructLevelMin),maximalSupportedLevel(){};
    ConstructSet(ConstructLevelType level){};
	ConstructSet(ConstructLevelType minLevel, ConstructLevelType maxLevel){};        
	ConstructLevelType getConstructLevel() { return ConstructLevelMin; };
	ConstructLevelType getMaxConstructLevel() { return ConstructLevelMin; };
	ConstructLevelType getMinConstructLevel() { return ConstructLevelMin; };
	void setCurrentMinLevel(ConstructLevelType minLevel){};
	void setCurrentMaxLevel(ConstructLevelType maxLevel){};

	void clear(){};
	size_t size(){
		return 0;
	}

	void add(ConstructSet * setB)
	{
	}
	void add(ConstructSet set)
	{
	}
	protected:
			ConstructLevelType minimalSupportedLevel,maximalSupportedLevel;
};

} // End Namespace Core
} // End namespace InstRO
#endif
