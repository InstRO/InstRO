#ifndef INSTRO_ANALYSIS_INTERFACE
#define INSTRO_ANALYSIS_INTERFACE

#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
#include "instro/core/ConstructSet.h"
#include "instro/tooling/NamedConstructAccess.h"

namespace InstRO {
namespace Tooling {
namespace NamedConstructAccess {
// CI: The specification and interfaces for the NamedConstructAccess methods are found in
// "instro/tooling/NamedConstructAccess.h"
}

namespace ControlFlowGraph {
class ControlFlowGraphNode {
 public:
	// CI: Explicit RAW Pointer. We do not release control of this CS
	virtual Core::ConstructSet *getAssociatedConstructSet() = 0;
};
class ControlFlowGraph {};
}
namespace ConstructElevator {
/* CI: The ConstructSet class is intended to be specialized for each compiler interface. It provides the basic
 * mechanisms to specify what construct level are contained. */
class ConstructElevator {
 public:
	// This is the implicit way that the PassManager will allways apply
	virtual std::unique_ptr<Core::ConstructSet> raise(Core::ConstructSet *input, Core::ConstructLevelType cl) = 0;
	// This is an explicit function used in very rare circumstances by e.g. a specialized selection pass (if at all)
	virtual std::unique_ptr<Core::ConstructSet> lower(Core::ConstructSet *input, Core::ConstructLevelType cl) = 0;
	// Crop Construct up to, or down to a level
	virtual std::unique_ptr<Core::ConstructSet> crop(Core::ConstructSet *input, Core::ConstructLevelType min,
																									 Core::ConstructLevelType max) = 0;
};
}
namespace GrammarInterface {
typedef enum {
	For,
	OpenMP,
	DoWhile,
	Expression,
	Return,
	Operator,
} GrammarTypesType;

class GrammarInterface {
	// class ConstructSetToGrammarTypeMapper
	virtual std::list<GrammarTypesType> getGrammerTypes(const Core::ConstructSet &cs) = 0;

	// class RequestCSByGrammarTypeInterface
	virtual std::unique_ptr<Core::ConstructSet> getConstructsByType(const GrammarTypesType &types) = 0;
};
}
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

class AnalysisManager {
 public:
	// CI: Explicit RawPointer to the Class. We do not release ownership
	virtual ExtendedCallGraph::ExtendedCallGraph *getECG() = 0;
	// CI:  Explicit RawPointer to the Class. We do not release ownership
	virtual ControlFlowGraph::ControlFlowGraph *getCFG() = 0;
	// CI:  Explicit RawPointer to the Class. We do not release ownership
	virtual ConstructElevator::ConstructElevator *getCSElevator() = 0;
	// CI:  Explicit RawPointer to the Class. We do not release ownership
	virtual GrammarInterface::GrammarInterface *getGrammarInterface() = 0;
	// CI:  Explicit RawPointer to the Class. We do not release ownership
	virtual NamedConstructAccess::NamedConstructAccess *getNamedConstructAccessFacility() = 0;
};

//  extern std::shared_ptr<AnalysisManager> analysisManager;
}
}
#endif
