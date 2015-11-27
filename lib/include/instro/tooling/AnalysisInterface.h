#ifndef INSTRO_TOOLING_ANALYSIS_INTERFACE_H
#define INSTRO_TOOLING_ANALYSIS_INTERFACE_H


#include "instro/tooling/NamedConstructAccess.h"
#include "instro/tooling/ConstructElevator.h"
#include "instro/tooling/ExtendedCallGraph.h"
#include "instro/tooling/ConstructClassInterface.h"
#include "instro/tooling/ControlFlowGraph.h"

namespace InstRO {
namespace Tooling {

namespace ExtendedCallGraph {}
namespace ControlFlowGraph {}
namespace ConstructElevator {}
namespace ConstructClassInterface {}
namespace NamedConstructAccess {}

class AnalysisManager {
 public:
	virtual ~AnalysisManager() {}

	// CI: Explicit RawPointer to the Class. We do not release ownership
	virtual ExtendedCallGraph::ExtendedCallGraph *getECG() = 0;
	// CI:  Explicit RawPointer to the Class. We do not release ownership
	virtual ControlFlowGraph::ControlFlowGraph *getCFG() = 0;
	// CI:  Explicit RawPointer to the Class. We do not release ownership
	virtual ConstructElevator::ConstructElevator *getCSElevator() = 0;
	// CI:  Explicit RawPointer to the Class. We do not release ownership
	virtual ConstructClassInterface::ConstructClassInterface *getGrammarInterface() = 0;
	// CI:  Explicit RawPointer to the Class. We do not release ownership
	virtual NamedConstructAccess::NamedConstructAccess *getNamedConstructAccessFacility() = 0;
};

}
}
#endif
