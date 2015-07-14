#ifndef ROSE_CONTROL_FLOW_GRAPH_H
#define ROSE_CONTROL_FLOW_GRAPH_H

#include <set>
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

class RoseCFGGenerator {
public:
	RoseCFGGenerator(SgProject* project) {
		for (auto funcDef : SageInterface::querySubTree<SgFunctionDefinition>(project, V_SgFunctionDefinition)) {
			generate(funcDef);
		}
	}



	InstRO::Tooling::ControlFlowGraph::ControlFlowGraph* getGraph() {
		return new InstRO::Tooling::ControlFlowGraph::ControlFlowGraph();
	}

private:
	std::map<std::shared_ptr<InstRO::Core::Construct>, BoostCFG> cfgs;

	void generate(SgFunctionDefinition* funcDef) {
		CFGNode startNode = VirtualCFG::cfgBeginningOfConstruct(funcDef);
		std::queue<CFGNode> workList;
		workList.push(startNode);
		std::set<CFGNode> visitedNodes;

		BoostCFG cfg;
		CFGConstructSetGenerator gen;

		while (!workList.empty()) {
			auto node = workList.front();
			workList.pop();

			if (visitedNodes.find(node) == visitedNodes.end()) {

				visitedNodes.insert(node);

				if (node.isInteresting() || isSgBasicBlock(node.getNode())) {
					std::cout << std::endl << "visit: " << node.getNode()->class_name() << std::endl
							<< node.toString() << std::endl;

					gen.reset(node.getIndex());
					node.getNode()->accept(gen);
					InstRO::Core::ConstructSet* cs = gen.getConstructSet();
					CFGNodeType nodeType = gen.getNodeType();

					if (cs != nullptr) {
						std::cout << "----> valid node." << std::endl;
						cfg.addNode(*cs, nodeType);
					}
				}

				for (auto outEdge : node.outEdges()) {
					workList.push(outEdge.target());
				}
			}
		}

		auto startConstruct = InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(funcDef);
		cfgs[startConstruct] = cfg;

		///XXX Print that stuff
		std::cout << std::endl;
		Graph::vertex_iterator vertexIter, vertexEnd;
		for (tie(vertexIter, vertexEnd) = boost::vertices(cfg.graph); vertexIter != vertexEnd; vertexIter++)
		{
			ControlFlowGraphNode node = cfg.graph.graph()[*vertexIter];
			std::cout << node << std::endl;
		}
		std::cout << boost::num_vertices(cfg.graph) << " vertices" << std::endl;

	}

};

}
}
}
}

#endif // ROSE_CONTROL_FLOW_GRAPH_H
