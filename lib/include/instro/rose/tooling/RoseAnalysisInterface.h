#include "rose.h"

#include "instro/tooling/AnalysisInterface.h"
#include "instro/rose/tooling/RoseNamedConstructAccess.h"

namespace InstRO{
	namespace Rose{
		namespace Tooling{
			class RoseAnalysisManager : public InstRO::Tooling::AnalysisManager
			{
			protected:
				SgProject * project;
				RoseAnalysisManager() = delete;
				InstRO::Rose::Tooling::NamedConstructAccess::RoseNamedConstructAccess * nca;
			public:
				RoseAnalysisManager(SgProject * proj) :project(proj), nca(new InstRO::Rose::Tooling::NamedConstructAccess::RoseNamedConstructAccess(proj)){};
				virtual InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph * getECG() override{
					throw std::string("Not IMplemented");
					return NULL;
				};
				virtual virtual InstRO::Tooling::ControlFlowGraph::ControlFlowGraph * getCFG() override{
					throw std::string("Not IMplemented");
					return NULL;
				};
				virtual virtual InstRO::Tooling::ConstructElevator::ConstructElevator * getCSElevator() override{
					throw std::string("Not IMplemented");
					return NULL;
				};
				virtual virtual InstRO::Tooling::GrammarInterface::GrammarInterface *getGrammarInterface() override{
					throw std::string("Not IMplemented");
					return NULL;
				};
				virtual InstRO::Tooling::NamedConstructAccess::NamedConstructAccess * getNamedConstructAccessFacility() override {
					return nca;
				}
			};
		}
	}
}