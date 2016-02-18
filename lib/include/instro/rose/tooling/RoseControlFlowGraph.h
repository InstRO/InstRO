#ifndef INSTRO_ROSE_TOOLING_ROSE_CONTROL_FLOW_GRAPH_H
#define INSTRO_ROSE_TOOLING_ROSE_CONTROL_FLOW_GRAPH_H

#include <set>
#include <map>
#include <queue>
#include <rose.h>

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

using namespace InstRO::Tooling::ControlFlowGraph;

class CFGConstructSetGenerator : public ROSE_VisitorPatternDefaultBase {
 public:
	CFGConstructSetGenerator(unsigned int index)
			: cs(new InstRO::Core::ConstructSet()), nodeType(NOT_SET), magicIndexVariable(index) {}

	InstRO::Core::ConstructSet* getConstructSet() { return cs; }
	CFGNodeType getNodeType() { return nodeType; }

	// FUNCTION ENTER or EXIT
	void visit(SgFunctionDefinition* node) {
		auto construct = InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node);
		InfrastructureInterface::ConstructSetCompilerInterface csci(cs);
		csci.put(construct);

		Sg_File_Info* fileInfo;
		if (magicIndexVariable == 0) {
			nodeType = FUNC_ENTRY;
			fileInfo = node->get_body()->get_startOfConstruct();
		} else if (magicIndexVariable == 3) {
			nodeType = FUNC_EXIT;
			fileInfo = node->get_body()->get_endOfConstruct();
		} else {
			logIt(ERROR) << "CFGConstructSetGenerator: encountered Function with invalid index" << std::endl;
			exit(1);
		}
		// we always have to have an associated node in order for the construct to work in the construct elevator
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, fileInfo));
	}

	// conditionals
	void visit(SgIfStmt* node) { invalidate(node); }
	void visit(SgSwitchStatement* node) { invalidate(node); }

	// loops
	void visit(SgForStatement* node) { invalidate(node); }
	void visit(SgWhileStmt* node) { invalidate(node); }
	void visit(SgDoWhileStmt* node) { invalidate(node); }

	// scopes
	void visit(SgBasicBlock* node) {
		if (!Core::RoseConstructTraitPredicates::CTScopeStatementPredicate()(node)) {
			invalidate(node);
			return;
		}

		InfrastructureInterface::ConstructSetCompilerInterface csci(cs);
		if (magicIndexVariable == 0) {
			nodeType = SCOPE_ENTRY;
			csci.put(
					InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, node->get_startOfConstruct()));
		} else if (magicIndexVariable == node->cfgIndexForEnd()) {
			nodeType = SCOPE_EXIT;
			csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, node->get_endOfConstruct()));
		} else {
			invalidate(node);
		}
	}

	void visit(SgVariableDeclaration* node) {
		if (!InstRO::Rose::Core::RoseConstructTraitPredicates::DefinedVariableDeclarationPredicate()(node)) {
			invalidate(node);
			return;
		}

		nodeType = STMT;
		InfrastructureInterface::ConstructSetCompilerInterface csci(cs);
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
	}

	void visit(SgForInitStatement* node) {
		invalidate(node);
	}

	// statements
	void visit(SgStatement* node) {
		nodeType = STMT;
		InfrastructureInterface::ConstructSetCompilerInterface csci(cs);
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
	}

	// expressions
	void visit(SgExpression* node) {
		if (Core::RoseConstructTraitPredicates::ExpressionInLoopOrConditionalHeader()(node)) {
			nodeType = EXPR;
			InfrastructureInterface::ConstructSetCompilerInterface csci(cs);
			csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
		} else {
			invalidate(node);
		}
	}

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
	RoseSingleFunctionCFGGenerator(SgFunctionDefinition* funcDef) {

		//XXX generate whole virtualcfg
		std::string name = funcDef->get_declaration()->get_name().getString();
//		VirtualCFG::cfgToDot(funcDef, "virtualcfg-"+name+".dot");

		auto cfgStartNode = aquireControlFlowGraphNode(funcDef->cfgForBeginning());
		cfg.setStartNode(cfgStartNode);
		cfg.addNode(cfgStartNode);

		auto cfgEndNode = aquireControlFlowGraphNode(funcDef->cfgForEnd());
		cfg.setEndNode(cfgEndNode);
		cfg.addNode(cfgEndNode);

		for (auto outEdge : funcDef->cfgForBeginning().outEdges()) {
			auto childCfgNode = outEdge.target();
			generate(cfgStartNode.getAssociatedConstructSet(), childCfgNode);
		}

		///XXX dump cfg
//		cfg.print(name+".dot");
//		logIt(INFO) << "RoseControlFlowGraph: " << boost::num_vertices(cfg.getGraph()) << " vertices and "
//				<< boost::num_edges(cfg.getGraph()) << " edges" << std::endl;
	}

	BoostCFG getCFG() { return std::move(cfg); }

 private:
	BoostCFG cfg;
	std::set<CFGNode> visitedCFGNodes;
	std::map<CFGNode, ControlFlowGraphNode> mapping;

	void generate(InstRO::Core::ConstructSet* previousNode, CFGNode vcfgNode) {
		InstRO::Core::ConstructSet* lastValidConstructSet = previousNode;
		if (vcfgNode.isInteresting() || isSgBasicBlock(vcfgNode.getNode())) {
			auto currentNode = aquireControlFlowGraphNode(vcfgNode);
			auto currentNodeCS = currentNode.getAssociatedConstructSet();
			if (currentNodeCS != nullptr && !currentNodeCS->empty()) {
				lastValidConstructSet = currentNodeCS;

				cfg.addNode(currentNode);
				cfg.addEdge(*previousNode, *currentNodeCS);

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
			CFGConstructSetGenerator gen(cfgNode.getIndex());

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
