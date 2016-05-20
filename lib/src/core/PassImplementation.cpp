#include "instro/core/PassImplementation.h"
#include "instro/Pass.h"

namespace InstRO {
namespace Core {

const InstRO::Core::ConstructSet *PassImplementation::getInput(int channel) { return managingPass->getInput(channel); }
}
}
