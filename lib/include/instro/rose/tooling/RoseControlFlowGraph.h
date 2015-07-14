#ifndef ROSE_CONTROL_FLOW_GRAPH_H
#define ROSE_CONTROL_FLOW_GRAPH_H

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
	CFGConstructSetGenerator() :
			cs(new InstRO::Core::ConstructSet()),
			nodeType(EXPR),
			magicIndexVariable(0),
			functionScope(nullptr) {
	}	// XXX EXPR is a bad default

	void reset(unsigned int index) {
		cs = new InstRO::Core::ConstructSet();
		nodeType = EXPR;
		magicIndexVariable = index;
	}

	InstRO::Core::ConstructSet* getConstructSet() {
		return cs;
	}
	CFGNodeType getNodeType() {
		return nodeType;
	}

	// FUNCTION ENTER or EXIT
	void visit(SgFunctionDefinition* node) {
		auto construct = InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node);
		InfracstructureInterface::ConstructSetCompilerInterface csci(cs);
		csci.put(construct);

		Sg_File_Info* fileInfo;
		if (magicIndexVariable == 0) {
			functionScope = node->get_body();

			nodeType = FUNC_ENTRY;
			fileInfo = node->get_body()->get_startOfConstruct();
		} else if (magicIndexVariable ==3) {
			nodeType = FUNC_EXIT;
			fileInfo = node->get_body()->get_endOfConstruct();
		} else {
			std::cout << "CFGConstructSetGenerator: encountered Function with invalid index" << std::endl;
			exit(1);
		}
		csci.put(std::make_shared<InstRO::Rose::Core::RoseFragment>(InstRO::Rose::Core::RoseFragment(fileInfo)));

	}

	// conditionals
	void visit(SgIfStmt* node) {
		SgStatement* condition = node->get_conditional();
		InfracstructureInterface::ConstructSetCompilerInterface csci(cs);


		auto construct = InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(condition);

		csci.put(construct);
		nodeType = STMT;
	}
	void visit(SgSwitchStatement* node) {
		nodeType = STMT;
	}

	// loops
	void visit(SgForStatement* node) {
		nodeType = STMT;
	}
	void visit(SgWhileStmt* node) {
		nodeType = STMT;
	}
	void visit(SgDoWhileStmt* node) {
		nodeType = STMT;
	}

	// scopes
	void visit(SgBasicBlock* node) {
		if (node == functionScope) {
			invalidate(node);
			return;
		}

		InfracstructureInterface::ConstructSetCompilerInterface csci(cs);
		if (magicIndexVariable == 0) {

			nodeType = SCOPE_ENTRY;
			csci.put(std::make_shared<InstRO::Rose::Core::RoseFragment>(InstRO::Rose::Core::RoseFragment(node->get_startOfConstruct())));
		} else if (magicIndexVariable == node->cfgIndexForEnd()) {
			nodeType = SCOPE_EXIT;
			csci.put(std::make_shared<InstRO::Rose::Core::RoseFragment>(InstRO::Rose::Core::RoseFragment(node->get_endOfConstruct())));
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
	void visit(SgExpression* node) {
		invalidate(node);
	}
	void visit(SgFunctionParameterList* node) {
		invalidate(node);
	}

	void visit(SgScopeStatement* node) {
		invalidate(node);
	}
	void visit(SgNode* node) {
		invalidate(node);
	}

private:
	InstRO::Core::ConstructSet* cs;
	CFGNodeType nodeType;

	unsigned magicIndexVariable;	// this index variable hold info of the rose VirtualCFG::CFGNode
	SgBasicBlock* functionScope;	// function definition is visited before the other nodes

	void invalidate(SgNode* node) {
		cs = nullptr;
	}
};

}
}
}
}

//// XXX BGL

#if 0

#include <iostream>
#include <map>
#include <set>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>

using namespace boost;

typedef labeled_graph<adjacency_list<vecS, vecS, directedS, ACFGNode>, ConstructSet> Graph;

// may not contain the same ConstructSet twice
class BoostACFG {
public:
	BoostACFG() {
	}

	void addNode(ConstructSet cs, ACFGNodeType type) {
		graph.add_vertex(cs);
		auto& graphNode = graph[cs];
		graphNode = ACFGNode(cs, type);
	}

	void addEdge(ConstructSet from, ConstructSet to) {
		add_edge_by_label(from, to, graph);
	}

	std::vector<ACFGNode> findNodes(const ConstructSet& constructSet) {
		std::vector<ACFGNode> foundNodes;

		// TODO

		return foundNodes;
	}

public:	//
	Graph graph;
};


void foo(int i) {}

int main(int argc, char** argv) {

	SgProject* project = frontend(argc, argv);

	for (auto funcDef : SageInterface::querySubTree<SgFunctionDefinition>(project, V_SgFunctionDefinition)) {

		BoostACFG g;

		VirtualCFG::interestingCfgToDot(funcDef, funcDef->get_declaration()->get_name().getString()+".dot");
		VirtualCFG::cfgToDot(funcDef, funcDef->get_declaration()->get_name().getString()+"-all.dot");


		CFGNode startNode = VirtualCFG::cfgBeginningOfConstruct(funcDef);
		std::queue<CFGNode> workList;
		workList.push(startNode);
		std::set<CFGNode> visitedNodes;

		CFGConstructSetGenerator gen;

		while (!workList.empty()) {
			auto node = workList.front();
			workList.pop();

			if (visitedNodes.find(node) == visitedNodes.end()) {

				visitedNodes.insert(node);

				if (node.isInteresting() || isSgBasicBlock(node.getNode())) {
					std::cout << std::endl <<  "visit: " << node.getNode()->class_name() << std::endl
							<< node.toString() << std::endl;

					gen.reset(node.getIndex());
					node.getNode()->accept(gen);
					ConstructSet* cs = gen.getConstructSet();
					ACFGNodeType nodeType = gen.getNodeType();

					if (cs != nullptr) {
						std::cout << "----> valid node." << std::endl;
						g.addNode(*cs, nodeType);
					}
				}

				for (auto outEdge : node.outEdges()) {
					workList.push(outEdge.target());
				}
			}
		}

		std::cout << std::endl << std::endl;

		Graph::vertex_iterator vertexIter, vertexEnd;
		for (tie(vertexIter, vertexEnd) = vertices(g.graph); vertexIter != vertexEnd; vertexIter++)
		{
			ACFGNode node = g.graph.graph()[*vertexIter];
			std::cout << node << std::endl;
		}

		std::cout << num_vertices(g.graph) << " vertices" << std::endl;
	}

}

#endif


#endif // ROSE_CONTROL_FLOW_GRAPH_H
