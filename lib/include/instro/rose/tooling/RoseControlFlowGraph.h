#ifndef ROSE_CONTROL_FLOW_GRAPH_H
#define ROSE_CONTROL_FLOW_GRAPH_H

#include <set>
#include <queue>
#include <rose.h>

#include "instro/rose/core/RoseConstructSet.h"
#include "instro/tooling/ControlFlowGraph.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace ControlFlowGraph {

using namespace InstRO::Tooling::ControlFlowGraph;

class CFGConstructSetGenerator : public ROSE_VisitorPatternDefaultBase {
 public:
	CFGConstructSetGenerator()
			: cs(new InstRO::Core::ConstructSet()),
				nodeType(EXPR),
				magicIndexVariable(0) {}	// XXX EXPR is a bad default

	void calibrate(unsigned int index) {
		magicIndexVariable = index;
	}

	InstRO::Core::ConstructSet* getConstructSet() { return cs; }
	CFGNodeType getNodeType() { return nodeType; }

	// FUNCTION ENTER or EXIT
	void visit(SgFunctionDefinition* node) {
		auto construct = InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node);
		InfracstructureInterface::ConstructSetCompilerInterface csci(cs);
		csci.put(construct);

		Sg_File_Info* fileInfo;
		if (magicIndexVariable == 0) {

			nodeType = FUNC_ENTRY;
			fileInfo = node->get_body()->get_startOfConstruct();
		} else if (magicIndexVariable == 3) {
			nodeType = FUNC_EXIT;
			fileInfo = node->get_body()->get_endOfConstruct();
		} else {
			std::cout << "CFGConstructSetGenerator: encountered Function with invalid index" << std::endl;
			exit(1);
		}
		// we always have to have an associated node in order for the construct to work in the construct elevator
		csci.put(std::make_shared<InstRO::Rose::Core::RoseFragment>(InstRO::Rose::Core::RoseFragment(node, fileInfo)));
	}

	// conditionals
	void visit(SgIfStmt* node) {
		invalidate(node);
	}
	void visit(SgSwitchStatement* node) {
		invalidate(node);
	}

	// loops
	void visit(SgForStatement* node) {
		invalidate(node);
	}
	void visit(SgWhileStmt* node) {
		invalidate(node);
	}
	void visit(SgDoWhileStmt* node) {
		invalidate(node);
	}

	// scopes
	void visit(SgBasicBlock* node) {

		if (!Core::RoseConstructLevelPredicates::CLScopeStatementPredicate()(node)) {
			invalidate(node);
			return;
		}

		InfracstructureInterface::ConstructSetCompilerInterface csci(cs);
		if (magicIndexVariable == 0) {
			nodeType = SCOPE_ENTRY;
			csci.put(std::make_shared<InstRO::Rose::Core::RoseFragment>(
					InstRO::Rose::Core::RoseFragment(node, node->get_startOfConstruct())));
		} else if (magicIndexVariable == node->cfgIndexForEnd()) {
			nodeType = SCOPE_EXIT;
			csci.put(std::make_shared<InstRO::Rose::Core::RoseFragment>(
					InstRO::Rose::Core::RoseFragment(node, node->get_endOfConstruct())));
		} else {
			invalidate(node);
		}
	}

	void visit(SgVariableDeclaration* node) {
		if (node->get_definition() == nullptr) {
			invalidate(node);
			return;
		}

		nodeType = STMT;
		InfracstructureInterface::ConstructSetCompilerInterface csci(cs);
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
	}

	// statements
	// TODO: any other statements that are not simple?
	void visit(SgStatement* node) {
		nodeType = STMT;
		InfracstructureInterface::ConstructSetCompilerInterface csci(cs);
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
	}

	// expressions
	void visit(SgExpression* node) { invalidate(node); }
	void visit(SgFunctionParameterList* node) { invalidate(node); }

	void visit(SgScopeStatement* node) { invalidate(node); }
	void visit(SgNode* node) { invalidate(node); }

 private:
	InstRO::Core::ConstructSet* cs;
	CFGNodeType nodeType;

	unsigned magicIndexVariable;	// this index variable hold info of the rose VirtualCFG::CFGNode

	void invalidate(SgNode* node) { cs = nullptr; }
};

class RoseSingleFunctionCFGGenerator {
 public:
	RoseSingleFunctionCFGGenerator(SgFunctionDefinition* startNode) {

		//XXX generate whole virtualcfg
		VirtualCFG::cfgToDot(startNode, "virtualcfg-"+startNode->get_declaration()->get_name().getString()+".dot");

		generate(nullptr, startNode->cfgForBeginning());

		std::cout << boost::num_vertices(cfg.graph) << " vertices" << std::endl;
		std::cout << boost::num_edges(cfg.graph) << " edges" << std::endl;
	}

	BoostCFG getCFG() {
		return cfg;
	}

 private:
	BoostCFG cfg;

	std::set<CFGNode> visitedCFGNodes;

	void generate(InstRO::Core::ConstructSet* previousNode, CFGNode cfgNode) {

		if (visitedCFGNodes.find(cfgNode) != visitedCFGNodes.end()) {
			return;
		} else {
			visitedCFGNodes.insert(cfgNode);
		}

		InstRO::Core::ConstructSet* previous = previousNode;
		if (cfgNode.isInteresting() || isSgBasicBlock(cfgNode.getNode())) {
			auto newACFGNode = aquireControlFlowGraphNode(cfgNode);

			if (newACFGNode.getAssociatedConstructSet() != nullptr) {

				///XXX
				std::cout << std::endl
						<< "visit: " << cfgNode.getNode()->class_name() << std::endl
						<< cfgNode.toString() << std::endl;

				cfg.addNode(newACFGNode);
				previous = newACFGNode.getAssociatedConstructSet();
				if (previousNode != nullptr) {
					cfg.addEdge(*previousNode, *newACFGNode.getAssociatedConstructSet());
				}
			}
		}

		for (auto outEdge : cfgNode.outEdges()) {
			auto childCfgNode = outEdge.target();

			// TODO only add non visited nodes

			generate(previous, childCfgNode);
		}

	}

	ControlFlowGraphNode aquireControlFlowGraphNode(CFGNode cfgNode) {
		CFGConstructSetGenerator gen;

		gen.calibrate(cfgNode.getIndex());
		cfgNode.getNode()->accept(gen);
		InstRO::Core::ConstructSet* cs = gen.getConstructSet();
		CFGNodeType nodeType = gen.getNodeType();

		return ControlFlowGraphNode(cs, nodeType);
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
		return new InstRO::Tooling::ControlFlowGraph::AbstractControlFlowGraph(cfgs);
	}

 private:
	std::vector<BoostCFG> cfgs;
};

}	// namespace ControlFlowGraph
}	// namespace Tooling
}	// namespace Rose
}	// namespace InstRO

#endif	// ROSE_CONTROL_FLOW_GRAPH_H
