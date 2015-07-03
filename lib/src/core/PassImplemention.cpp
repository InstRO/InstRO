#include "instro/core/PassImplementation.h"

namespace InstRO{
namespace Core{
	InstRO::Core::ConstructSet * PassImplementation::getInput(Pass * pId)
{
			return pId->getOutput();
}
}
}