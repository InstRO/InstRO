#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
#include <set>
#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Tooling {
namespace ExtendedCallGraph {
class ExtendedCallGraphNode {
public:
	// Explicit RAW Pointer. We do not release control of this CS
	virtual Core::ConstructSet *getAssociatedConstructSet() = 0;
};

class ExtendedCallGraph {
 public:
	//    virtual std::set<ExtendedCallGraphNode *> findNodes(Core::ConstructSet *cs) = 0;


	// Get ECG nodes for the constructs in the 
	virtual std::set<ExtendedCallGraphNode *> getEXGNodes(Core::ConstructSet *cs) = 0;
};

}	// ExtendedCallGraph
}	// Tooling
}	// InstRO
