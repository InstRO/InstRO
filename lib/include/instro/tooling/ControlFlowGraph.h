#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Tooling {
namespace ControlFlowGraph {
class ControlFlowGraphNode {
 public:
	// CI: Explicit RAW Pointer. We do not release control of this CS
	virtual Core::ConstructSet *getAssociatedConstructSet() = 0;
};
class ControlFlowGraph {};
}
}
}