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
	RoseAnalysisManager(SgProject *proj) :
			project(proj),
			nca(new NamedConstructAccess::RoseNamedConstructAccess(proj)),
			ce(new ConstructElevator::ConstructElevator()),
			gi(new GrammarInterface::RoseGrammarInterface(proj)) {}

	RoseAnalysisManager() = delete;
	~RoseAnalysisManager() {
		delete nca;
		delete ce;
		delete gi;
	}

	virtual InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph *getECG() override {
		throw std::string("Not Implemented");
		return NULL;
	}
	virtual InstRO::Tooling::ControlFlowGraph::ControlFlowGraph *getCFG() override {
		throw std::string("Not Implemented");
		return NULL;
	}
	virtual InstRO::Tooling::ConstructElevator::ConstructElevator *getCSElevator() override {
		return ce;
	}
	virtual InstRO::Tooling::GrammarInterface::GrammarInterface *getGrammarInterface() override {
		return gi;
	}
	virtual InstRO::Tooling::NamedConstructAccess::NamedConstructAccess *getNamedConstructAccessFacility() override {
		return nca;
	}

 protected:
	SgProject *project;
	NamedConstructAccess::RoseNamedConstructAccess *nca;
	ConstructElevator::ConstructElevator *ce;
	GrammarInterface::RoseGrammarInterface *gi;
};

}	// Tooling
}	// Rose
}	// InstRO
