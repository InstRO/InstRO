#ifndef INSTRO_ROSE_ROSEPASS_IMPLEMENTATION_H
#define INSTRO_ROSE_ROSEPASS_IMPLEMENTATION_H

#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Rose {

class RosePassImplementation : public InstRO::Core::PassImplementation {
 public:
	RosePassImplementation(InstRO::Core::ChannelConfiguration channelConfig) : PassImplementation(channelConfig) {}
};

}
}
#endif
