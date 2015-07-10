#include <memory>
#include "instro/core/Helper.h"
#include "instro/core/ConstructSet.h"
#include "instro/tooling/AnalysisInterface.h"
#include "instro/tooling/NamedConstructAccess.h"
#include "instro/example/ExampleNamedConstructAccess.h"
#include "instro/example/ExampleConstructSet.h"

namespace InstRO {
namespace Example {
namespace Tooling {

/*
namespace NamedConstructAccess
{
	class ExampleNamedConstructAccess :public InstRO::Tooling :: NamedConstructAccess::NamedConstructAccess{
		InstRO::Core::ConstructSet getConstructsByIdentifyerName(InstRO::Tooling::NamedConstructAccess::Matcher &){ return
InstRO::Core::ConstructSet(); }
		// b) contents of strings
		InstRO::Core::ConstructSet getConstructsByUserTextStringMatch(InstRO::Tooling::NamedConstructAccess::Matcher &){
return InstRO::Core::ConstructSet(); };
	};
}*/

class ExampleControlFlowGraph : public InstRO::Tooling::ControlFlowGraph::ControlFlowGraph {};

/* CI: The ConstructSet class is intended to be specialized for each compiler interface. It provides the basic
 * mechanisms to specify what construct level are contained. */
class ExampleConstructElevator : public InstRO::Tooling::ConstructElevator::ConstructElevator {
 public:
	// This is the implicit way, that the PassManager will allways apply
	virtual std::unique_ptr<InstRO::Core::ConstructSet> raise(InstRO::Core::ConstructSet *input,
																														InstRO::Core::ConstructTraitType cl) override {
		throw std::string("ExampleConstructElevator::raise : Not Implemented");
		return std::make_unique<InstRO::Core::ConstructSet>(*input);
	}
	// This is an explicit function used in very rare circumstances by e.g. a specialized selection pass (if at all)
	virtual std::unique_ptr<InstRO::Core::ConstructSet> lower(InstRO::Core::ConstructSet *input,
																														InstRO::Core::ConstructTraitType cl) {
		throw std::string("ExampleConstructElevator::lower : Not Implemented");
		return std::make_unique<InstRO::Core::ConstructSet>(*input);
	}
};

class ExampleGrammarInterface : public InstRO::Tooling::GrammarInterface::GrammarInterface {
 public:
	// class ConstructSetToGrammarTypeMapper
	virtual std::list<InstRO::Tooling::GrammarInterface::GrammarTypesType> getGrammerTypes(
			const InstRO::Core::ConstructSet &cs) {
		throw std::string("ExampleGrammarInterface::getGrammerTypes : Not Implemented");
		return std::list<InstRO::Tooling::GrammarInterface::GrammarTypesType>();
	}

	// class RequestCSByGrammarTypeInterface
	virtual std::unique_ptr<InstRO::Core::ConstructSet> getConstructsByType(
			const InstRO::Tooling::GrammarInterface::GrammarTypesType &types) override {
		throw std::string("ExampleGrammarInterface::getConstructsByType : Not Implemented");
		return std::make_unique<InstRO::Core::ConstructSet>(InstRO::Core::ConstructSet());
	};
};

class ExampleExtendedCallGraph : public InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph {
 public:
	std::vector<InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode *> findNodes(
			InstRO::Core::ConstructSet *cs) override {
		return std::vector<InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode *>();
	};
	// std::vector<ExtendedCallGraphNode*> findNodes(GrammarInterface::GrammerTypes type);
};

class ExampleAnalysisManager : public InstRO::Tooling::AnalysisManager {
 protected:
	ExampleExtendedCallGraph *ecg;
	ExampleConstructElevator *ce;
	ExampleControlFlowGraph *cfg;
	NamedConstructAccess::ExampleNamedConstructAccess *nca;

 public:
	ExampleAnalysisManager() : ecg(NULL), ce(NULL), cfg(NULL), nca(NULL){};

	ExampleExtendedCallGraph *getECG() {
		if (ecg == NULL)
			ecg = new ExampleExtendedCallGraph();
		return ecg;
	};
	ExampleControlFlowGraph *getCFG() { return new ExampleControlFlowGraph(); };
	ExampleConstructElevator *getCSElevator() { return new ExampleConstructElevator(); }
	ExampleGrammarInterface *getGrammarInterface() { return new ExampleGrammarInterface(); }
	InstRO::Tooling::NamedConstructAccess::NamedConstructAccess *getNamedConstructAccessFacility() {
		if (nca == NULL)
			nca = new NamedConstructAccess::ExampleNamedConstructAccess();
		return nca;
	};
};
}
}
}
