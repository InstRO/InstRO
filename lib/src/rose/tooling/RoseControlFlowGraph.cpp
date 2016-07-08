#include "instro/rose/tooling/RoseControlFlowGraph.h"
namespace InstRO {
namespace Rose {
namespace Tooling {
namespace ControlFlowGraph {

// FUNCTION ENTER or EXIT
void CFGConstructSetGenerator::visit(SgFunctionDefinition* node) {
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

// scopes
void CFGConstructSetGenerator::visit(SgBasicBlock* node) {
	if (!Core::RoseConstructTraitPredicates::CTScopeStatementPredicate()(node)) {
		invalidate(node);
		return;
	}

	InfrastructureInterface::ConstructSetCompilerInterface csci(cs);
	if (magicIndexVariable == 0) {
		nodeType = SCOPE_ENTRY;
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, node->get_startOfConstruct()));
	} else if (magicIndexVariable == node->cfgIndexForEnd()) {
		nodeType = SCOPE_EXIT;
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, node->get_endOfConstruct()));
	} else {
		invalidate(node);
	}
}

void CFGConstructSetGenerator::visit(SgVariableDeclaration* node) {
	if (!InstRO::Rose::Core::RoseConstructTraitPredicates::DefinedVariableDeclarationPredicate()(node)) {
		invalidate(node);
		return;
	}

	nodeType = STMT;
	InfrastructureInterface::ConstructSetCompilerInterface csci(cs);
	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
}

// expressions
void CFGConstructSetGenerator::visit(SgExpression* node) {
	if (Core::RoseConstructTraitPredicates::ExpressionInLoopOrConditionalHeader()(node)) {
		nodeType = EXPR;
		InfrastructureInterface::ConstructSetCompilerInterface csci(cs);
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
	} else {
		invalidate(node);
	}
}

/**
 * Rose Single Function CFG Generator
 */
RoseSingleFunctionCFGGenerator::RoseSingleFunctionCFGGenerator(SgFunctionDefinition* funcDef) {
	// XXX generate whole virtualcfg
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

	/// XXX dump cfg
	//		cfg.print(name+".dot");
	//		logIt(INFO) << "RoseControlFlowGraph: " << boost::num_vertices(cfg.getGraph()) << " vertices and "
	//				<< boost::num_edges(cfg.getGraph()) << " edges" << std::endl;
}

void RoseSingleFunctionCFGGenerator::generate(InstRO::Core::ConstructSet* previousNode, CFGNode vcfgNode) {
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
}
}
}
}
