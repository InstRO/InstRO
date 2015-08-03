#ifndef INSTRO_ROSE_TOOLING_ROSE_ANALYSIS_INTERFACE
#define INSTRO_ROSE_TOOLING_ROSE_ANALYSIS_INTERFACE

#include "rose.h"

#include "instro/tooling/AnalysisInterface.h"
#include "instro/rose/tooling/RoseNamedConstructAccess.h"
#include "instro/rose/tooling/RoseControlFlowGraph.h"
#include "instro/rose/tooling/RoseExtendedCallGraph.h"
#include "instro/rose/tooling/RoseGrammarInterface.h"
#include "instro/rose/tooling/RoseConstructElevator.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
class RoseAnalysisManager : public InstRO::Tooling::AnalysisManager {
 public:
	RoseAnalysisManager(SgProject *proj)
			: project(proj),
				namedConstructAccessInstance(new NamedConstructAccess::RoseNamedConstructAccess(proj)),
				ce(new ConstructElevator::ConstructElevator()),
				gi(new GrammarInterface::RoseGrammarInterface(proj)),
				controlFlowGraph((new ControlFlowGraph::RoseCFGGenerator(proj))->getGraph()),
				extendedCallGraph((new ExtendedCallGraph::RoseECGGenerator())->build(proj)) {}

	RoseAnalysisManager() = delete;
	~RoseAnalysisManager() {
		delete namedConstructAccessInstance;
		delete ce;
		delete gi;
		delete controlFlowGraph;
		delete extendedCallGraph;
	}

	virtual InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph *getECG() override { return extendedCallGraph; }
	virtual InstRO::Tooling::ControlFlowGraph::ControlFlowGraph *getCFG() override { return controlFlowGraph; }
	virtual InstRO::Tooling::ConstructElevator::ConstructElevator *getCSElevator() override { return ce; }
	virtual InstRO::Tooling::GrammarInterface::GrammarInterface *getGrammarInterface() override { return gi; }
	virtual InstRO::Tooling::NamedConstructAccess::NamedConstructAccess *getNamedConstructAccessFacility() override {
		return namedConstructAccessInstance;
	}

 protected:
	SgProject *project;
	NamedConstructAccess::RoseNamedConstructAccess *namedConstructAccessInstance;
	ConstructElevator::ConstructElevator *ce;
	GrammarInterface::RoseGrammarInterface *gi;
	ControlFlowGraph::ControlFlowGraph *controlFlowGraph;
	ExtendedCallGraph::ExtendedCallGraph *extendedCallGraph;
};

}	// Tooling
}	// Rose
}	// InstRO

#endif // INSTRO_ROSE_TOOLING_ROSE_ANALYSIS_INTERFACE
