#include "instro/core/PassImplementation.h"
#include "instro/core/Pass.h"

namespace InstRO {
namespace Core {

InstRO::Core::ConstructSet *PassImplementation::getInput(Pass *pId) { return pId->getOutput(); }

const InstRO::Core::ConstructSet *PassImplementation::getInput(int channel) { return managingPass->getInput(channel); }
}
}
