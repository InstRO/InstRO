#ifndef INSTRO_CORE_CONSTRUCTSET_H
#define INSTRO_CORE_CONSTRUCTSET_H 0.1

#include <vector>

namespace InstRO{
class Pass;
namespace Core{
typedef enum ContstructLevelEnum{
	ConstructLevelMin=1,
	ConstructLevelLiteral=1,
	ConstructLevelExpression=2,
	ConstructLevelStatement,
	MaxConstructLevel=1
}ContstructLevelType;

std::string contstructLevelToString(ContstructLevelType type);
std::string operator+(const std::string &lhs,const ContstructLevelEnum &type);
/* CI: The ConstructSet class is intended to be specialized for each compiler interface. It provides the basic mechanisms to specify what construct level are contained. */
class ConstructSet
{
	public:
		ConstructSet(){};
		ContstructLevelType getConstructLevel(){return ConstructLevelMin;};
		ContstructLevelType getMaxConstructLevel(){return ConstructLevelMin;};
		ContstructLevelType getMinConstructLevel(){return ConstructLevelMin;};
		ConstructSet(ContstructLevelType level){};
		ConstructSet(ContstructLevelType minLevel,ContstructLevelType maxLevel){};
		void setCurrentMinLevel(ContstructLevelType minLevel){};
		void setCurrentMaxLevel(ContstructLevelType maxLevel){};
};
}
} // End namespace InstRO

#endif