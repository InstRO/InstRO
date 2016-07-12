#include "instro/rose/tooling/RoseExtendedCallGraph.h"
#include "instro/rose/core/RoseConstructPredicates.h"

#include "instro/utility/Logger.h"

using namespace InstRO::Rose::Tooling::ExtendedCallGraph;

void RoseECGConstructSetGenerator::visit(SgFunctionDefinition* node) {
	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
	nodeType = ECGNodeType::FUNCTION;
}

void RoseECGConstructSetGenerator::visit(SgFunctionDeclaration* node) {
	assert(node);

	if (node->get_definition() != nullptr) {
		visit(node->get_definition());
		return;
	}

	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, node->get_startOfConstruct()));
	nodeType = ECGNodeType::FUNCTION;
}

void RoseECGConstructSetGenerator::visit(SgFunctionCallExp* node) {
	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
	nodeType = ECGNodeType::FUNCTION_CALL;
}
void RoseECGConstructSetGenerator::visit(SgIfStmt* node) {
	addConditionStmtOrExpr(node->get_conditional());
	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));	// whole conditional
	nodeType = ECGNodeType::CONDITIONAL;
}
void RoseECGConstructSetGenerator::visit(SgSwitchStatement* node) {
	addConditionStmtOrExpr(node->get_item_selector());
	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));	// whole conditional
	nodeType = ECGNodeType::CONDITIONAL;
}
void RoseECGConstructSetGenerator::visit(SgForStatement* node) {
	auto initStmts = node->get_for_init_stmt()->get_init_stmt();
	for (auto initStmt : initStmts) {
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(initStmt));
	}
	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node->get_test()));
	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node->get_increment()));

	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));	// whole loop

	nodeType = ECGNodeType::LOOP;
}
void RoseECGConstructSetGenerator::visit(SgWhileStmt* node) {
	addConditionStmtOrExpr(node->get_condition());
	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));	// whole loop

	nodeType = ECGNodeType::LOOP;
}
void RoseECGConstructSetGenerator::visit(SgDoWhileStmt* node) {
	SgExpression* conditionExpr = isSgExprStatement(node->get_condition())->get_expression();
	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(conditionExpr));
	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));	// whole loop
	nodeType = ECGNodeType::LOOP;
}
void RoseECGConstructSetGenerator::visit(SgBasicBlock* node) {
	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, node->get_startOfConstruct()));
	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getFragment(node, node->get_endOfConstruct()));

	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));	// whole scope

	nodeType = ECGNodeType::SCOPE;
}

void RoseECGConstructSetGenerator::visit(SgNode* n) {
	// XXX this should not happen
	logIt(ERROR) << "RoseECGConstructSetGenerator Error. Got invalid node" << std::endl << n->unparseToString() << std::endl;
	exit(1);
}

void RoseECGConstructSetGenerator::addConditionStmtOrExpr(SgStatement* conditionStmt) {
	if (InstRO::Rose::Core::RoseConstructTraitPredicates::CTSimpleStatementPredicate()(conditionStmt)) {
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(conditionStmt));
	} else {
		SgExpression* conditionalExpr = isSgExprStatement(conditionStmt)->get_expression();
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(conditionalExpr));
	}
}

ExtendedCallGraph* RoseECGGenerator::build(SgProject* project) {
	std::vector<SgFunctionDefinition*> funcDefs =
			SageInterface::querySubTree<SgFunctionDefinition>(project, V_SgFunctionDefinition);
	for (SgFunctionDefinition* funcDef : funcDefs) {
		// visit function definition
		traverse(funcDef);
	}

	/// XXX
	callgraph->print("extendedCallGraph.dot");

	return callgraph;
}

ExtendedCallGraphNode* RoseECGGenerator::getDefiningECGNode(SgFunctionDefinition* node) {
	RoseECGConstructSetGenerator genCS;
	node->accept(genCS);

	std::string mangledName = node->get_declaration()->get_mangled_name();
	if (uniqueDecls.find(mangledName) != uniqueDecls.end()) {
		if (uniqueDecls[mangledName]->get_definition() == nullptr) {
			auto ecgNode = genCS.getECGNode();
			callgraph->swapConstructSet(uniqueNodes[mangledName]->getAssociatedConstructSet(),
																	ecgNode->getAssociatedConstructSet());
		}

	} else {
		// add node so it is not missing, if it has no children
		auto ecgNode = genCS.getECGNode();

		callgraph->addNode(ecgNode);
		uniqueDecls[mangledName] = node->get_declaration();
		uniqueNodes[mangledName] = ecgNode;
	}

	return uniqueNodes[mangledName];
}

ExtendedCallGraphNode* RoseECGGenerator::getDefiningECGNode(SgFunctionDeclaration* node) {
	std::string mangledName = node->get_mangled_name();
	if (uniqueDecls.find(mangledName) == uniqueDecls.end()) {
		RoseECGConstructSetGenerator genCS;
		node->accept(genCS);
		auto ecgNode = genCS.getECGNode();

		callgraph->addNode(ecgNode);
		uniqueDecls[mangledName] = node;
		uniqueNodes[mangledName] = ecgNode;
	}
	return uniqueNodes[mangledName];
}

void RoseECGGenerator::preOrderVisit(SgFunctionDefinition* node) {
	auto ecgNode = getDefiningECGNode(node);

	currentVisitNode.push(ecgNode);
}
void RoseECGGenerator::preOrderVisit(SgFunctionCallExp* node) {
	RoseECGConstructSetGenerator genCS;
	node->accept(genCS);

	// enclosing node -> callExp
	auto ecgNode = genCS.getECGNode();
	callgraph->addEdge(currentVisitNode.top(), ecgNode);

	// don't push current node
	// funcCalls can't have children expect their corresponding funcDecl

	SgFunctionDeclaration* calledDecl = node->getAssociatedFunctionDeclaration();
	if (calledDecl == nullptr) {
		logIt(WARN) << "Virtual call in line " << node->get_startOfConstruct()->get_line() << ": "
								<< node->unparseToString() << std::endl;

		auto arrowExp = isSgArrowExp(node->get_function());
		if (arrowExp == nullptr) {
			return;
		}

		auto memberFuncRefExp = isSgMemberFunctionRefExp(arrowExp->get_rhs_operand());
		if (memberFuncRefExp == nullptr) {
			return;
		}

		calledDecl = memberFuncRefExp->getAssociatedMemberFunctionDeclaration();
		if (calledDecl == nullptr) {
			logIt(WARN) << "No associated function declaration for: " << node->unparseToString() << std::endl;
			return;	// FIXME MZ: calledDecl is not always available
		}
	}

	SgFunctionDefinition* calledDef = tryGetDefiningDeclaration(calledDecl)->get_definition();
	if (calledDef == nullptr) {
		auto ecgNodeCall = getDefiningECGNode(calledDecl);
		callgraph->addEdge(ecgNode, ecgNodeCall);
	} else {
		auto ecgNodeCall = getDefiningECGNode(calledDef);
		callgraph->addEdge(ecgNode, ecgNodeCall);
	}
}

void RoseECGGenerator::preOrderVisit(SgBasicBlock* node) {
	// skip fileScope
	if (!InstRO::Rose::Core::RoseConstructTraitPredicates::CTScopeStatementPredicate()(node)) {
		return;
	}
	defaultPreOrderBehavior(node);
}
void RoseECGGenerator::postOrderVisit(SgBasicBlock* node) {
	if (!InstRO::Rose::Core::RoseConstructTraitPredicates::CTScopeStatementPredicate()(node)) {
		return;	// skip fileScope
	}
	currentVisitNode.pop();
}

void RoseECGGenerator::preOrderVisit(SgNode* node) {
	// manual dispatch -_-
	if (isSgFunctionDefinition(node)) {
		preOrderVisit(isSgFunctionDefinition(node));
	} else if (isSgFunctionCallExp(node)) {
		preOrderVisit(isSgFunctionCallExp(node));
	} else if (isSgIfStmt(node)) {
		preOrderVisit(isSgIfStmt(node));
	} else if (isSgSwitchStatement(node)) {
		preOrderVisit(isSgSwitchStatement(node));
	} else if (isSgForStatement(node)) {
		preOrderVisit(isSgForStatement(node));
	} else if (isSgWhileStmt(node)) {
		preOrderVisit(isSgWhileStmt(node));
	} else if (isSgDoWhileStmt(node)) {
		preOrderVisit(isSgDoWhileStmt(node));
	} else if (isSgBasicBlock(node) && Core::RoseConstructTraitPredicates::CTScopeStatementPredicate()(node)) {
		preOrderVisit(isSgBasicBlock(node));
	}
}
void RoseECGGenerator::postOrderVisit(SgNode* node) {
	// manual dispatch -_-
	if (isSgFunctionDefinition(node)) {
		postOrderVisit(isSgFunctionDefinition(node));
	} else if (isSgFunctionCallExp(node)) {
		postOrderVisit(isSgFunctionCallExp(node));
	} else if (isSgIfStmt(node)) {
		postOrderVisit(isSgIfStmt(node));
	} else if (isSgSwitchStatement(node)) {
		postOrderVisit(isSgSwitchStatement(node));
	} else if (isSgForStatement(node)) {
		postOrderVisit(isSgForStatement(node));
	} else if (isSgWhileStmt(node)) {
		postOrderVisit(isSgWhileStmt(node));
	} else if (isSgDoWhileStmt(node)) {
		postOrderVisit(isSgDoWhileStmt(node));
	} else if (isSgBasicBlock(node) && Core::RoseConstructTraitPredicates::CTScopeStatementPredicate()(node)) {
		postOrderVisit(isSgBasicBlock(node));
	}
}

void RoseECGGenerator::defaultPreOrderBehavior(SgNode* node) {
	RoseECGConstructSetGenerator genCS;
	node->accept(genCS);

	callgraph->addEdge(currentVisitNode.top(), genCS.getECGNode());
	currentVisitNode.push(callgraph->getNodeWithExactConstructSet(genCS.getConstructSet()));
}

SgFunctionDeclaration* RoseECGGenerator::getDefiningDeclaration(SgFunctionDeclaration* oldDecl) {
	SgDeclarationStatement* definingDecl = oldDecl->get_definingDeclaration();
	return isSgFunctionDeclaration(definingDecl);
}

SgFunctionDeclaration* RoseECGGenerator::tryGetDefiningDeclaration(SgFunctionDeclaration* oldDecl) {
	SgFunctionDeclaration* definingDecl = getDefiningDeclaration(oldDecl);
	if (definingDecl) {
		return definingDecl;
	}
	return oldDecl;
}

