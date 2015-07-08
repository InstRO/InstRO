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
 protected:
	SgProject *project;
	RoseAnalysisManager() = delete;
	InstRO::Rose::Tooling::NamedConstructAccess::RoseNamedConstructAccess *nca;
	InstRO::Rose::Tooling::ConstructElevator::ConstructElevator *ce;

 public:
	RoseAnalysisManager(SgProject *proj)
			: project(proj),
				nca(new InstRO::Rose::Tooling::NamedConstructAccess::RoseNamedConstructAccess(proj)),
				ce(new InstRO::Rose::Tooling::ConstructElevator::ConstructElevator()){};
	~RoseAnalysisManager() {
		delete nca;
		delete ce;
	}
	virtual InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph *getECG() override {
		throw std::string("Not IMplemented");
		return NULL;
	};
	virtual InstRO::Tooling::ControlFlowGraph::ControlFlowGraph *getCFG() override {
		throw std::string("Not IMplemented");
		return NULL;
	};
	virtual InstRO::Tooling::ConstructElevator::ConstructElevator *getCSElevator() override { return ce; };
	virtual InstRO::Tooling::GrammarInterface::GrammarInterface *getGrammarInterface() override {
		throw std::string("Not IMplemented");
		return NULL;
	};
	virtual InstRO::Tooling::NamedConstructAccess::NamedConstructAccess *getNamedConstructAccessFacility() override {
		return nca;
	}
};
}
}
}
