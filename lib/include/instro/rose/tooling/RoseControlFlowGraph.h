#ifndef INSTRO_ROSE_TOOLING_ROSE_CONTROL_FLOW_GRAPH_H
#define INSTRO_ROSE_TOOLING_ROSE_CONTROL_FLOW_GRAPH_H

#include <map>
#include <queue>
#include <rose.h>
#include <set>

#include "instro/rose/core/RoseConstructSet.h"
#include "instro/tooling/ControlFlowGraph.h"
#include "instro/utility/Logger.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace ControlFlowGraph {

/**
 * TODO: RN 2015-07
 * - handle empty scopes correctly
 * - handle that return statements are never in a scope
 */

// The alias allows convenient access to members of the abstract type / interfaces
namespace BCFG = InstRO::Tooling::ControlFlowGraph;

class CFGConstructSetGenerator : public ROSE_VisitorPatternDefaultBase {
 public:
	CFGConstructSetGenerator(unsigned int index)
			: cs(new InstRO::Core::ConstructSet()), nodeType(BCFG::NOT_SET), magicIndexVariable(index) {}

	InstRO::Core::ConstructSet* getConstructSet() { return cs; }
	BCFG::CFGNodeType getNodeType() { return nodeType; }

	// FUNCTION ENTER or EXIT
	void visit(SgFunctionDefinition* node);

	// conditionals
	void visit(SgIfStmt* node) { invalidate(node); }
	void visit(SgSwitchStatement* node) { invalidate(node); }

	// scopes
	void visit(SgBasicBlock* node);

	// loops
	void visit(SgForStatement* node) { invalidate(node); }
	void visit(SgWhileStmt* node) { invalidate(node); }
	void visit(SgDoWhileStmt* node) { invalidate(node); }

	void visit(SgVariableDeclaration* node);
	void visit(SgForInitStatement* node) { invalidate(node); }

	// statements
	void visit(SgStatement* node) {
		nodeType = BCFG::STMT;
		InfrastructureInterface::ConstructSetCompilerInterface csci(cs);
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
	}

	// expressions
	void visit(SgExpression* node);

	void visit(SgFunctionParameterList* node) { invalidate(node); }

	void visit(SgScopeStatement* node) { invalidate(node); }
	void visit(SgNode* node) { invalidate(node); }

 private:
	InstRO::Core::ConstructSet* cs;
	BCFG::CFGNodeType nodeType;

	unsigned magicIndexVariable;	// this index variable hold info of the rose VirtualCFG::CFGNode

	void invalidate(SgNode* node) { cs = nullptr; }
};

class RoseSingleFunctionCFGGenerator {
 public:
	RoseSingleFunctionCFGGenerator(SgFunctionDefinition* funcDef);

	BCFG::BoostCFG getCFG() { return std::move(cfg); }

 private:
	BCFG::BoostCFG cfg;
	std::set<CFGNode> visitedCFGNodes;
	std::map<CFGNode, BCFG::ControlFlowGraphNode> mapping;

	void generate(InstRO::Core::ConstructSet* previousNode, CFGNode vcfgNode);

	BCFG::ControlFlowGraphNode aquireControlFlowGraphNode(CFGNode cfgNode) {
		if (mapping.find(cfgNode) == mapping.end()) {
			CFGConstructSetGenerator gen(cfgNode.getIndex());

			cfgNode.getNode()->accept(gen);
			mapping[cfgNode] = BCFG::ControlFlowGraphNode(gen.getConstructSet(), gen.getNodeType());
		}
		return mapping[cfgNode];
	}
};

class RoseCFGGenerator {
 public:
	RoseCFGGenerator(SgProject* project) {
		for (auto funcDef : SageInterface::querySubTree<SgFunctionDefinition>(project, V_SgFunctionDefinition)) {
			RoseSingleFunctionCFGGenerator singleFunctionGen(funcDef);
			cfgs.push_back(singleFunctionGen.getCFG());
		}
	}

	InstRO::Tooling::ControlFlowGraph::ControlFlowGraph* getGraph() {
		return new InstRO::Tooling::ControlFlowGraph::AbstractControlFlowGraph(std::move(cfgs));
	}

 private:
	std::vector<BCFG::BoostCFG> cfgs;
};

}	// namespace ControlFlowGraph
}	// namespace Tooling
}	// namespace Rose
}	// namespace InstRO

#endif	// ROSE_CONTROL_FLOW_GRAPH_H
