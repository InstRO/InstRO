#ifndef INSTRO_ANALYSIS_INTERFACE
#define INSTRO_ANALYSIS_INTERFACE

#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
#include "instro/core/ConstructSet.h"

#include "instro/tooling/NamedConstructAccess.h"
#include "instro/tooling/ConstructElevator.h"
#include "instro/tooling/ExtendedCallGraph.h"
#include "instro/tooling/GrammarInterface.h"
#include "instro/tooling/ControlFlowGraph.h"

namespace InstRO {
namespace Tooling {



// CI: The specification and interfaces for the NamedConstructAccess methods are found in
// "instro/tooling/NamedConstructAccess.h"

namespace ExtendedCallGraph{};
namespace ControlFlowGraph{};
namespace ConstructElevator {};
namespace GrammarInterface{};
namespace NamedConstructAccess {};



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
