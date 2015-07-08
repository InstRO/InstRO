#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Tooling {
namespace ExtendedCallGraph {
class ExtendedCallGraphNode {
	// Explicit RAW Pointer. We do not release control of this CS
	virtual Core::ConstructSet *getAssociatedConstructSet() = 0;
};
class ExtendedCallGraph {
 public:
	virtual std::vector<ExtendedCallGraphNode *> findNodes(Core::ConstructSet *cs) = 0;
	// std::vector<ExtendedCallGraphNode*> findNodes(GrammarInterface::GrammerTypes type);
};
}
}
}
