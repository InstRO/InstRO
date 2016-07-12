#ifndef INSTRO_ROSE_TOOLING_ROSE_ANALYSIS_INTERFACE_H
#define INSTRO_ROSE_TOOLING_ROSE_ANALYSIS_INTERFACE_H

#include "rose.h"

#include "instro/tooling/AnalysisInterface.h"
#include "instro/rose/tooling/RoseNamedConstructAccess.h"
#include "instro/rose/tooling/RoseControlFlowGraph.h"
#include "instro/rose/tooling/RoseExtendedCallGraph.h"
#include "instro/rose/tooling/RoseConstructTraitInterface.h"
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
				cti(new ConstructTraitInterface::RoseConstructTraitInterface(proj)),
				controlFlowGraph((new ControlFlowGraph::RoseCFGGenerator(proj))->getGraph()),
				extendedCallGraph((new ExtendedCallGraph::RoseECGGenerator())->build(proj)) {}

	RoseAnalysisManager() = delete;
	~RoseAnalysisManager() {
		delete namedConstructAccessInstance;
		delete ce;
		delete cti;
		delete controlFlowGraph;
		delete extendedCallGraph;
	}

	virtual InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph *getECG() override { return extendedCallGraph; }
	virtual InstRO::Tooling::ControlFlowGraph::ControlFlowGraph *getCFG() override { return controlFlowGraph; }
	virtual InstRO::Tooling::ConstructElevator::ConstructElevator *getCSElevator() override { return ce; }
	virtual InstRO::Tooling::ConstructTraitInterface::ConstructTraitInterface *getConstructTraitInterface() override {
		return cti;
	}
	virtual InstRO::Tooling::NamedConstructAccess::NamedConstructAccess *getNamedConstructAccessFacility() override {
		return namedConstructAccessInstance;
	}

 protected:
	SgProject *project;
	NamedConstructAccess::RoseNamedConstructAccess *namedConstructAccessInstance;
	ConstructElevator::ConstructElevator *ce;
	ConstructTraitInterface::RoseConstructTraitInterface *cti;
	InstRO::Tooling::ControlFlowGraph::ControlFlowGraph *controlFlowGraph;
	ExtendedCallGraph::ExtendedCallGraph *extendedCallGraph;
};

}	// Tooling
}	// Rose
}	// InstRO

#endif	// INSTRO_ROSE_TOOLING_ROSE_ANALYSIS_INTERFACE
