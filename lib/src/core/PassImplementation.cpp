#include "instro/core/PassImplementation.h"
#include "instro/core/Pass.h"

namespace InstRO {
namespace Core {

InstRO::Core::ConstructSet *InstRO::Core::ChannelConfiguration::operator[](int pos) {
	Pass *p = inputChannelPasses.at(pos);
	return managingPass->getInput(p);
}

InstRO::Core::ConstructSet *PassImplementation::getInput(Pass *pId) { return pId->getOutput(); }

InstRO::Core::ConstructSet *PassImplementation::getInput(int channel) { return channelConfig[channel]; }
}
}
