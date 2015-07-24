#ifndef ROSE_CONTROL_FLOW_GRAPH_H
#define ROSE_CONTROL_FLOW_GRAPH_H

#include <set>
#include <map>
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
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, fileInfo));
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
			csci.put(
					InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, node->get_startOfConstruct()));
		} else if (magicIndexVariable == node->cfgIndexForEnd()) {
			nodeType = SCOPE_EXIT;
			csci.put(
					InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, node->get_endOfConstruct()));
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
		std::string name = startNode->get_declaration()->get_name().getString();
		VirtualCFG::cfgToDot(startNode, "virtualcfg-"+name+".dot");

		generate(nullptr, startNode->cfgForBeginning());

		///XXX
		cfg.print(name+".dot");
		std::cout << boost::num_vertices(cfg.graph) << " vertices" << std::endl;
		std::cout << boost::num_edges(cfg.graph) << " edges" << std::endl;
	}

	BoostCFG getCFG() {
		return std::move(cfg);
	}

 private:
	BoostCFG cfg;
	std::set<CFGNode> visitedCFGNodes;
	std::map<CFGNode, ControlFlowGraphNode> mapping;

	void generate(InstRO::Core::ConstructSet* previousNode, CFGNode vcfgNode) {


		InstRO::Core::ConstructSet* lastValidConstructSet = previousNode;
		if (vcfgNode.isInteresting() || isSgBasicBlock(vcfgNode.getNode())) {

			auto currentNode = aquireControlFlowGraphNode(vcfgNode);
			auto currentNodeCS = currentNode.getAssociatedConstructSet();
			if (currentNodeCS != nullptr) {
				lastValidConstructSet = currentNodeCS;

				///XXX
				std::cout << std::endl << "visit: " << vcfgNode.getNode()->class_name() << std::endl
						<< vcfgNode.toString() << std::endl;

				cfg.addNode(currentNode);
				if (previousNode != nullptr) {
					cfg.addEdge(*previousNode, *currentNodeCS);
				}

				if (visitedCFGNodes.find(vcfgNode) != visitedCFGNodes.end()) {
					return;
				} else {
					visitedCFGNodes.insert(vcfgNode);
				}
			}
		}

		for (auto outEdge : vcfgNode.outEdges()) {
			auto childCfgNode = outEdge.target();
			generate(lastValidConstructSet, childCfgNode);
		}
	}

	ControlFlowGraphNode aquireControlFlowGraphNode(CFGNode cfgNode) {

		if (mapping.find(cfgNode) == mapping.end()) {

			CFGConstructSetGenerator gen;
			gen.calibrate(cfgNode.getIndex());

			cfgNode.getNode()->accept(gen);
			mapping[cfgNode] = ControlFlowGraphNode(gen.getConstructSet(), gen.getNodeType());
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
	std::vector<BoostCFG> cfgs;
};

}	// namespace ControlFlowGraph
}	// namespace Tooling
}	// namespace Rose
}	// namespace InstRO

#endif	// ROSE_CONTROL_FLOW_GRAPH_H
