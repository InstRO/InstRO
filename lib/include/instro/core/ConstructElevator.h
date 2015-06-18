#ifndef INSTRO_CORE_CONSTRUCTELEVATOR_H
#define INSTRO_CORE_CONSTRUCTELEVATOR_H 0.1

#include "instro/core/ConstructSet.h"

#include <vector>

namespace InstRO{
class Pass;
namespace Core{

/* CI: The ConstructSet class is intended to be specialized for each compiler interface. It provides the basic mechanisms to specify what construct level are contained. */
class ConstructElevator
{
	public:
		ConstructElevator(){};
		// This is the implicit way, that the PassManager will allways apply
		ConstructSet* elevateUp(ConstructSet* input,ContstructLevelEnum targetConstructLevel){return input;}
		// This is an explicit function used in very rare circumstances by e.g. a specialized selection pass (if at all)
		ConstructSet* elevateDown(ConstructSet* input,ContstructLevelEnum targetConstructLevel){return input;}
};
}
} // End namespace InstRO

#endif