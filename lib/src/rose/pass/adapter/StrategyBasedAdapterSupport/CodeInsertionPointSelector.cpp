#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/CodeInsertionPointSelector.h"

#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/instrobuilder.h"

#include <iostream>

#define FEW_ANCHORS 1

using namespace InstRO;

namespace { // anonymous local namespace

	// manage order of statements across multiple calls
	std::map<SgNode*, SgDeclarationStatement*> setupAnchor;
	std::map<SgNode*, SgDeclarationStatement*> initializationAnchor;
	std::map<SgNode*, SgDeclarationStatement*> entryAnchor;
	std::map<SgNode*, SgDeclarationStatement*> returnAnchor;
	std::map<SgNode*, SgDeclarationStatement*> endOfBlockAnchor;

	// manage list of function bodies to avoid insertion before body just because it was encapsulated by a try-catch
	std::set<SgNode*> functionBodies;
	std::map<SgFunctionDefinition*, SgBasicBlock*> functionBodyRelation;

	SgDeclarationStatement* buildHiddenAnchor(SgScopeStatement* scope) {

		std::string name = SageInterface::generateUniqueVariableName(scope, "anchor");

		SgType *type = SageBuilder::buildIntType();

		SgDeclarationStatement *st = SageBuilder::buildVariableDeclaration(name, type, NULL, scope);

//		st->get_file_info()->unsetOutputInCodeGeneration();

		return st;
	}


	// TODO refactor *Shutdown codes
	void _handleExceptionReturn(std::vector<std::pair<SgStatement*, CodeInsertionHelper::RelativePosition> >& stmtList, SgStatement* node) {

		if (SageInterface::getProject()->get_Cxx_only()) {

			// check cache
			auto it = returnAnchor.find(node);
			if (it != returnAnchor.end()) {
				stmtList.push_back(std::make_pair(it->second, CodeInsertionHelper::before));
				return;
			}

			// need to handle the special case, since an additional block is constructed as new function body
			SgFunctionDefinition* fnDef = isSgFunctionDefinition(node->get_parent());

			SgTryStmt* tryStmt = InstROBuilder::buildTryCatchStmt(node);

			SgCatchOptionStmt* catchOption = isSgCatchOptionStmt(tryStmt->get_catch_statement_seq().front());

			SgBasicBlock* catchBody = isSgBasicBlock(catchOption->get_body());

			SgStatement* rethrowStmt = InstROBuilder::buildRethrowStatement();

			// this is the backup point for shutdown code
			SgDeclarationStatement* anchor = buildHiddenAnchor(catchBody);
			SageInterface::appendStatement(anchor, catchBody);

			SageInterface::appendStatement(rethrowStmt, catchBody);

			stmtList.push_back(std::make_pair(anchor, CodeInsertionHelper::before));

			// update cache
			if (fnDef) {
				functionBodies.insert(node);
				returnAnchor.insert(std::make_pair(fnDef->get_body(), anchor));
			}
			returnAnchor.insert(std::make_pair(node, anchor));
		}
	}


	void _handleExceptionShutdown(std::vector<SgDeclarationStatement*>& stmtList, SgStatement* node) {

		if (SageInterface::getProject()->get_Cxx_only()) {

			// check cache
			auto it = returnAnchor.find(node);
			if (it != returnAnchor.end()) {
				stmtList.push_back(it->second);
				return;
			}

			// need to handle the special case, since an additional block is constructed as new function body
			SgFunctionDefinition* fnDef = isSgFunctionDefinition(node->get_parent());

			SgTryStmt* tryStmt = InstROBuilder::buildTryCatchStmt(node);

			SgCatchOptionStmt* catchOption = isSgCatchOptionStmt(tryStmt->get_catch_statement_seq().front());

			SgBasicBlock* catchBody = isSgBasicBlock(catchOption->get_body());

			SgStatement* rethrowStmt = InstROBuilder::buildRethrowStatement();

			// this is the backup point for shutdown code
			SgDeclarationStatement* anchor = buildHiddenAnchor(catchBody);
			SageInterface::appendStatement(anchor, catchBody);

			SageInterface::appendStatement(rethrowStmt, catchBody);

			stmtList.push_back(anchor);

			// update cache
			if (fnDef) {
				functionBodies.insert(node);
				returnAnchor.insert(std::make_pair(fnDef->get_body(), anchor));
			}
			returnAnchor.insert(std::make_pair(node, anchor));
		}
	}


	void _handleContinueStatements(std::vector<std::pair<SgStatement*, CodeInsertionHelper::RelativePosition> >& stmtList, SgStatement* node) {

		std::vector<SgContinueStmt*> continueStmtList = SageInterface::findContinueStmts(node);

		for (auto i : continueStmtList) {
			stmtList.push_back(std::make_pair(i, CodeInsertionHelper::before));
		}
	}


	void _handleBreakStatements(std::vector<std::pair<SgStatement*, CodeInsertionHelper::RelativePosition> >& stmtList, SgStatement* node) {

		std::vector<SgBreakStmt*> breakStmtList = SageInterface::findBreakStmts(node);

		for (auto i : breakStmtList) {
			stmtList.push_back(std::make_pair(i, CodeInsertionHelper::before));
		}
	}


	void _transformReturnHelper(SgReturnStmt* node) {

		SgExpression* ex = node->get_expression();

		// primities are not touched
		if (ex == NULL ||
			isSgValueExp(ex) ||
			isSgVarRefExp(ex)) {

			return;
		}

		SgScopeStatement* scope = SageInterface::getScope(node);

		std::pair<SgVariableDeclaration*, SgExpression*> replacement =
			SageInterface::createTempVariableForExpression(ex, scope, true, NULL);

		SageInterface::insertStatementBefore(node, replacement.first);

		SageInterface::replaceExpression(ex, replacement.second);
	}


	void _handleReturnStatements(std::vector<std::pair<SgStatement*, CodeInsertionHelper::RelativePosition> >& stmtList, SgStatement* node) {

		std::vector<SgReturnStmt*> retStmtList = SageInterface::querySubTree<SgReturnStmt>(node, V_SgReturnStmt);

		SgFunctionDefinition* fDef = SageInterface::getEnclosingFunctionDefinition(node, true);

		SgFunctionType* fType = fDef->get_declaration()->get_type();

		SgType* returnType = fType->get_return_type();

		for (auto i : retStmtList) {

			if (! isSgTypeVoid(returnType)) {

				_transformReturnHelper(i);
			}

			SgDeclarationStatement* shutdownAnchor = NULL;

			// check cache
			auto it = returnAnchor.find(i);
			if (it == returnAnchor.end()) {
				// this is the backup point for shutdown code
				shutdownAnchor = buildHiddenAnchor(SageInterface::getScope(i));
				SageInterface::insertStatementBefore(i, shutdownAnchor);
				returnAnchor.insert(std::make_pair(i, shutdownAnchor));
			}
			else {
				shutdownAnchor = it->second;
			}

			stmtList.push_back(std::make_pair(shutdownAnchor, CodeInsertionHelper::before));
		}
	}


	void _handleReturnShutdown(std::vector<SgDeclarationStatement*>& stmtList, SgStatement* node) {

		std::vector<SgReturnStmt*> retStmtList = SageInterface::querySubTree<SgReturnStmt>(node, V_SgReturnStmt);

		SgFunctionDefinition* fDef = SageInterface::getEnclosingFunctionDefinition(node, true);

		SgFunctionType* fType = fDef->get_declaration()->get_type();

		SgType* returnType = fType->get_return_type();

		for (auto i : retStmtList) {

			if (! isSgTypeVoid(returnType)) {

				_transformReturnHelper(i);
			}

			// check cache
			auto it = returnAnchor.find(i);
			if (it != returnAnchor.end()) {
				stmtList.push_back(it->second);
				continue;
			}

			SgScopeStatement* scope = SageInterface::getScope(i);

			// this is the backup point for shutdown code
			SgDeclarationStatement* anchor = buildHiddenAnchor(scope);
			SageInterface::insertStatementBefore(i, anchor);

			stmtList.push_back(anchor);

			returnAnchor.insert(std::make_pair(i, anchor));
		}
	}


	SgDeclarationStatement* _handleBasicBlockEntryPoint(SgBasicBlock* node) {

		if (node == NULL) return NULL;

		// check cache
		auto it = entryAnchor.find(node);
		if (it != entryAnchor.end()) {
			return it->second;
		}

		SgDeclarationStatement* anchor = buildHiddenAnchor(node);

		// Whenever an initialization is constructed, an entry placeholder is mandatory available!
		assert(initializationAnchor.find(node) == initializationAnchor.end());

		SageInterface::prependStatement(anchor, node);
		// caching
		entryAnchor.insert(std::make_pair(node, anchor));

		return anchor;
	}


	void _handleEndOfBasicBlock(std::vector<std::pair<SgStatement*, CodeInsertionHelper::RelativePosition> >& stmtList, SgBasicBlock* node) {

		// Return statements are already considered
		if (isSgReturnStmt(SageInterface::getLastStatement(node))) return;

		SgDeclarationStatement* shutdownAnchor = NULL;

		// check cache
		auto it = endOfBlockAnchor.find(node);
		if (it == endOfBlockAnchor.end()) {
			// this is the backup point for shutdown code
			shutdownAnchor = buildHiddenAnchor(node);
			SageInterface::appendStatement(shutdownAnchor, node);
			endOfBlockAnchor.insert(std::make_pair(node, shutdownAnchor));
		}
		else {
			shutdownAnchor = it->second;
		}

		stmtList.push_back(std::make_pair(shutdownAnchor, CodeInsertionHelper::before));
	}


	void _handleBasicBlockShutdown(std::vector<SgDeclarationStatement*>& stmtList, SgBasicBlock* node) {

		// Return statements are already considered
		if (isSgReturnStmt(SageInterface::getLastStatement(node))) return;

		// check cache
		auto it = endOfBlockAnchor.find(node);
		if (it != endOfBlockAnchor.end()) {
			stmtList.push_back(it->second);
			return;
		}

		// this is the backup point for shutdown code
		SgDeclarationStatement* anchor = buildHiddenAnchor(node);
		SageInterface::appendStatement(anchor, node);

		endOfBlockAnchor.insert(std::make_pair(node, anchor));
 
		stmtList.push_back(anchor);
	}


	void _handleBasicBlockExitPoints(std::vector<std::pair<SgStatement*, CodeInsertionHelper::RelativePosition> >& stmtList, SgBasicBlock* node) {
		if (node == NULL) return;

		_handleExceptionReturn(stmtList, node);

		_handleContinueStatements(stmtList, node);

		_handleBreakStatements(stmtList, node);

		_handleReturnStatements(stmtList, node);

		_handleEndOfBasicBlock(stmtList, node);
	}


	void _handleMainShutdownPoints(std::vector<SgDeclarationStatement*>& stmtList, SgBasicBlock* node) {
		if (node == NULL) return;

		_handleExceptionShutdown(stmtList, node);

		_handleReturnShutdown(stmtList, node);

		_handleBasicBlockShutdown(stmtList, node);
	}


	/**
	 * Used internally by _EnsureNthScopeAsBasicBlock function.
	 */
	class _EnsureNthScopeAsBasicBlockVisitor : public Visitor_R<SgBasicBlock*> {
	public:
		_EnsureNthScopeAsBasicBlockVisitor(size_t index = 0);

		void visit(SgBasicBlock* node);
		void visit(SgIfStmt* node);
		void visit(SgSwitchStatement* node);
		void visit(SgCaseOptionStmt* node);
		void visit(SgDefaultOptionStmt* node);
		void visit(SgWhileStmt* node);
		void visit(SgDoWhileStmt* node);
		void visit(SgForStatement* node);
		void visit(SgFunctionDeclaration* node);
		void visit(SgFunctionDefinition* node);

		void visitDefault(SgNode* node);

	private:
		size_t index;
	};

	_EnsureNthScopeAsBasicBlockVisitor::_EnsureNthScopeAsBasicBlockVisitor(size_t index)
		: index(index) {
	}

	void _EnsureNthScopeAsBasicBlockVisitor::visit(SgBasicBlock* node) {
		assert(index == 0);
		_result = node;
	}

	void _EnsureNthScopeAsBasicBlockVisitor::visit(SgIfStmt* node) {
		assert(index < 2);

		if (index == 0) {
			_result = SageInterface::ensureBasicBlockAsTrueBodyOfIf(node);
		}
		else {
			_result = SageInterface::ensureBasicBlockAsFalseBodyOfIf(node);
		}
	}

	void _EnsureNthScopeAsBasicBlockVisitor::visit(SgSwitchStatement* node) {

		std::vector<SgStatement*> cases = SageInterface::getSwitchCases(node);
		assert(index < cases.size());

		// visit SgCaseOptionStmt or SgDefaultOptionStmt
		cases[index]->accept(*this);
	}

	// TODO Analyze behavior in cases, where a case option is not terminated before next case option.
	void _EnsureNthScopeAsBasicBlockVisitor::visit(SgCaseOptionStmt* node) {

		_result = SageInterface::ensureBasicBlockAsBodyOfCaseOption(node);
	}

	void _EnsureNthScopeAsBasicBlockVisitor::visit(SgDefaultOptionStmt* node) {

		_result = SageInterface::ensureBasicBlockAsBodyOfDefaultOption(node);
	}

	void _EnsureNthScopeAsBasicBlockVisitor::visit(SgWhileStmt* node) {

		_result = SageInterface::ensureBasicBlockAsBodyOfWhile(node);
	}

	void _EnsureNthScopeAsBasicBlockVisitor::visit(SgDoWhileStmt* node) {

		_result = SageInterface::ensureBasicBlockAsBodyOfDoWhile(node);
	}

	void _EnsureNthScopeAsBasicBlockVisitor::visit(SgForStatement* node) {

		_result = SageInterface::ensureBasicBlockAsBodyOfFor(node);
	}

	void _EnsureNthScopeAsBasicBlockVisitor::visit(SgFunctionDeclaration* node) {
		assert(("Requires defining declaration!", node->get_definition() != NULL));

		// visit SgFunctionDefinition
		node->get_definition()->accept(*this);
	}

	void _EnsureNthScopeAsBasicBlockVisitor::visit(SgFunctionDefinition* node) {

		auto it = functionBodyRelation.find(node);
		if (it != functionBodyRelation.end()) {
			_result = it->second;
		}
		else {
			SgBasicBlock* body = node->get_body();
			functionBodyRelation.insert(std::make_pair(node, body));
			_result = body;
		}
	}

	// TODO possibly handle other constructs before throwing an error
	void _EnsureNthScopeAsBasicBlockVisitor::visitDefault(SgNode* node) {

		assert(("Unexpected statement input!", false));
	}


	/**
	 * Input is expected to be eigher a basic block or a statement which contains a form of body/bodies.
	 *
	 * If the body is not a basic block, it is converted to be one.
	 */
	SgBasicBlock* _ensureNthScopeAsBasicBlock(SgStatement* node, size_t index) {

		_EnsureNthScopeAsBasicBlockVisitor v(index);

		node->accept(v);

		return v.getResult();
	}

} // end anonymous namespace


CodeInsertionHelper InstRO::getMainGlobalDeclarationPoint(SgNode* node) {

	SgFunctionDeclaration* mainFn = SageInterface::findMain(SageInterface::getProject());

//	SgDeclarationStatement* anchor = buildHiddenAnchor(SageInterface::getScope(mainFn));

//	SageInterface::insertStatementBefore(mainFn, anchor);

	return CodeInsertionHelper(mainFn, CodeInsertionHelper::before);
}


CodeInsertionHelper InstRO::getMainSetupPoint(SgNode* node) {

	SgFunctionDeclaration* mainFn = SageInterface::findMain(SageInterface::getProject());

	SgBasicBlock* body = mainFn->get_definition()->get_body();

	auto it = setupAnchor.find(body);
	if (it != setupAnchor.end()) {
		return CodeInsertionHelper(it->second);
	}

	SgDeclarationStatement* anchor = buildHiddenAnchor(body);

	auto it2 = initializationAnchor.find(body);
	if (it2 == initializationAnchor.end()) {
		// add entry point anchor and initialization anchor to ensure setup before init
		SgDeclarationStatement* init = buildHiddenAnchor(body);
		auto it3 = entryAnchor.find(body);
		if (it3 == entryAnchor.end()) {
			// add entry point anchor to ensure initialization before entry
			SgDeclarationStatement* temp = _handleBasicBlockEntryPoint(body);
			SageInterface::insertStatementBefore(temp, init);
			entryAnchor.insert(std::make_pair(body, temp));
		}
		else {
			SageInterface::insertStatementBefore(it3->second, init);
		}
		SageInterface::insertStatementBefore(init, anchor);
		initializationAnchor.insert(std::make_pair(body, init));
	}
	else {
		SageInterface::insertStatementBefore(it2->second, anchor);
	}

	// caching
	setupAnchor.insert(std::make_pair(body, anchor));

	return CodeInsertionHelper(anchor);
}


CodeInsertionHelper InstRO::getMainInitializationPoint(SgNode* node) {

	SgFunctionDeclaration* mainFn = SageInterface::findMain(SageInterface::getProject());

	return getFunctionInitializationPoint(mainFn);
}


CodeInsertionHelper InstRO::getNearestGlobalDeclarationPoint(SgNode* node) {

	SgStatement* parent = SageInterface::getScope(node);
	SgStatement* node2 = SageInterface::getEnclosingNode<SgStatement>(node, true);

	while (parent && ! isSgGlobal(parent)) {
		node2 = parent;
		parent = SageInterface::getEnclosingNode<SgStatement>(node2);
	}

	SgDeclarationStatement* anchor = buildHiddenAnchor(SageInterface::getScope(node2));

	// into global scope, as close to the parameter node as possible
	// TODO: find a way to keep preprocessor information and still hide the anchor node
	SageInterface::insertStatementBefore(node2, anchor, true);

	return CodeInsertionHelper(anchor);
}


CodeInsertionHelper InstRO::getEarliestGlobalDeclarationPoint(SgNode* node) {

	SgGlobal* global = SageInterface::getGlobalScope(node);

	SgDeclarationStatement* anchor = buildHiddenAnchor(global);

	SageInterface::prependStatement(anchor, global);

	return CodeInsertionHelper(anchor);
}


CodeInsertionHelper InstRO::getFunctionInitializationPoint(SgNode* node) {

	SgFunctionDeclaration* fnDecl = SageInterface::getEnclosingFunctionDeclaration(node, true);

	SgBasicBlock* body = fnDecl->get_definition()->get_body();

	// check cache
	auto it = initializationAnchor.find(body);
	if (it != initializationAnchor.end()) {
		return CodeInsertionHelper(it->second);
	}

	SgDeclarationStatement* anchor = buildHiddenAnchor(body);

	auto it2 = entryAnchor.find(body);
	if (it2 == entryAnchor.end()) {
		// add entry point anchor to ensure initialization before entry
		SgDeclarationStatement* temp = _handleBasicBlockEntryPoint(body);
		SageInterface::insertStatementBefore(temp, anchor);
		entryAnchor.insert(std::make_pair(body, temp));
	}
	else {
		SageInterface::insertStatementBefore(it2->second, anchor);
	}

	// caching
	initializationAnchor.insert(std::make_pair(body, anchor));

	return CodeInsertionHelper(anchor);
}


CodeInsertionHelper InstRO::getStatementEntryPoint(SgStatement* node) {

	// TODO how to handle cases, where insert BEFORE function body is attempted and it somehow
	// works out because a previous function already enclosed the body in a surrounding try-catch?
	if (functionBodies.count(node) > 0) return CodeInsertionHelper(std::vector<SgDeclarationStatement*>());

	assert(!isSgBasicBlock(node) || functionBodies.count(node) == 0);

	assert(!isSgFunctionDefinition(node->get_parent()));

	// TODO how to handle SgReturnStatement? It breaks shutdown code when applied in main function
	// There may be other related problems for non-main returns
	assert(!isSgReturnStmt(node));

	return CodeInsertionHelper(node, CodeInsertionHelper::before);
}


CodeInsertionHelper InstRO::getStatementExitPoints(SgStatement* node) {

	if (functionBodies.count(node) > 0) return CodeInsertionHelper(std::vector<SgDeclarationStatement*>());

	assert(!isSgBasicBlock(node) || functionBodies.count(node) == 0);

	assert(!isSgFunctionDefinition(node->get_parent()));

	// caller responsibility to transform a return before instrumenting the expression
	// there is no valid way to instrument *behind* the return statement
	assert(!isSgReturnStmt(node));

	std::vector<std::pair<SgStatement*, CodeInsertionHelper::RelativePosition> > stmtList;

	_handleExceptionReturn(stmtList, node);

	_handleContinueStatements(stmtList, node);

	_handleBreakStatements(stmtList, node);

	_handleReturnStatements(stmtList, node);

	stmtList.push_back(std::make_pair(node, CodeInsertionHelper::after));

	return CodeInsertionHelper(stmtList);
}



CodeInsertionHelper InstRO::getNthInnerScopeEntryPoint(SgStatement* node, size_t index) {

	std::vector<SgDeclarationStatement*> stmtList;

	SgBasicBlock* target = _ensureNthScopeAsBasicBlock(node, index);

	SgDeclarationStatement* anchor = _handleBasicBlockEntryPoint(target);

	return CodeInsertionHelper(anchor);
}


CodeInsertionHelper InstRO::getNthInnerScopeExitPoints(SgStatement* node, size_t index) {

	std::vector<std::pair<SgStatement*, CodeInsertionHelper::RelativePosition> > stmtList;

	SgBasicBlock* target = _ensureNthScopeAsBasicBlock(node, index);

	assert(target);

	_handleBasicBlockExitPoints(stmtList, target);

	return CodeInsertionHelper(stmtList);
}


CodeInsertionHelper InstRO::getMainShutdownPoints(SgNode* node) {

	std::vector<SgDeclarationStatement*> stmtList;

	SgFunctionDeclaration* mainFn = SageInterface::findMain(SageInterface::getProject());

	_handleMainShutdownPoints(stmtList, mainFn->get_definition()->get_body());

	return CodeInsertionHelper(stmtList);
}


