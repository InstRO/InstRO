#ifndef INSTRO_CORE_CONSTRUCTSET_H
#define INSTRO_CORE_CONSTRUCTSET_H 0.1

#include <vector>
#include <string>

namespace InstRO{

//class Pass;

namespace Core{

typedef enum ContstructLevelEnum{
	ConstructLevelMin=1,
	ConstructLevelLiteral=1,
	ConstructLevelExpression=2,
	ConstructLevelStatement,
	MaxConstructLevel=1
}ConstructLevelType;

std::string contstructLevelToString(ConstructLevelType type);
std::string operator+(const std::string &lhs,const ConstructLevelType &type);
/* CI: The ConstructSet class is intended to be specialized for each compiler interface. It provides the basic mechanisms to specify what construct level are contained. */
class ConstructSet
{
	public:
		ConstructSet(){};
		ConstructLevelType getConstructLevel(){return ConstructLevelMin;};
		ConstructLevelType getMaxConstructLevel(){return ConstructLevelMin;};
		ConstructLevelType getMinConstructLevel(){return ConstructLevelMin;};
		ConstructSet(ConstructLevelType level){};
		ConstructSet(ConstructLevelType minLevel,ConstructLevelType maxLevel){};
		void setCurrentMinLevel(ConstructLevelType minLevel){};
		void setCurrentMaxLevel(ConstructLevelType maxLevel){};
};
}
} // End namespace InstRO

#endif
