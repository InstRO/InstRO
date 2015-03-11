#ifndef INSTRO_CORE_CONSTRUCTSET_H
#define INSTRO_CORE_CONSTRUCTSET_H 0.1

#include <string>

namespace InstRO {

namespace Core {

typedef enum ContstructLevelEnum {
	ConstructLevelMin = 1,
	ConstructLevelLiteral = 1,
	ConstructLevelExpression = 2,
	ConstructLevelStatement,
	MaxConstructLevel = 1
} ConstructLevelType;

std::string contstructLevelToString(ConstructLevelType type);
std::string operator+(const std::string &lhs, const ConstructLevelType &type);
/* CI: The ConstructSet class is intended to be specialized for each compiler
 * interface. It provides the basic mechanisms to specify what construct level
 * are contained. 
 */
class ConstructSet {
 public:
	ConstructSet(){};
	ConstructSet(ConstructLevelType level){};
	ConstructSet(ConstructLevelType minLevel, ConstructLevelType maxLevel){};

	ConstructLevelType getConstructLevel() { return ConstructLevelMin; };
	ConstructLevelType getMaxConstructLevel() { return ConstructLevelMin; };
	ConstructLevelType getMinConstructLevel() { return ConstructLevelMin; };
	void setCurrentMinLevel(ConstructLevelType minLevel){};
	void setCurrentMaxLevel(ConstructLevelType maxLevel){};
};
} // Core
}	// End namespace InstRO

#endif
