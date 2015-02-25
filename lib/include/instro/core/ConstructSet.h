#ifndef INSTRO_CORE_CONSTRUCTSET_H
#define INSTRO_CORE_CONSTRUCTSET_H 0.1

#include <vector>

namespace InstRO{
class Pass;
namespace Core{
typedef int ContstructLevelType;
/* CI: The ConstructSet class is intended to be specialized for each compiler interface. It provides the basic mechanisms to specify what construct level are contained. */
class ConstructSet
{
	public:
		ContstructLevelType getConstructLevel(){};
		ContstructLevelType getMaxConstructLevel(){};
		ContstructLevelType getMinConstructLevel(){};
		ConstructSet(ContstructLevelType level){};
		ConstructSet(ContstructLevelType minLevel,ContstructLevelType maxLevel){};
		ContstructLevelType setCurrentMinLevel(){};
		ContstructLevelType setCurrentMaxLevel(){};
};
}
} // End namespace InstRO

#endif