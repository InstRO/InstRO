#ifndef INSTRO_ROSE_ROSEPASS_H
#define INSTRO_ROSE_ROSEPASS_H

#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Rose {
class RosePassImplementation : public InstRO::Core::PassImplementation {
 public:
	RosePassImplementation(InstRO::Core::ChannelConfiguration ccfg) : PassImplementation(ccfg) {}
};

class RoseLegcyPass : public RosePassImplementation {};

namespace Adapter {
}
namespace Selector{
}
namespace Transformer {
}

}
};
#endif