/*
 * callfrequencyheuristicselector.cpp
 *
 *  Created on: Feb 13, 2014
 *      Author: rness
 */

#include "instro/selectors/callfrequencyheuristicselector.h"

#define VERBOSE 5

using namespace InstRO;

CallFrequencyHeuristicSelector::CallFrequencyHeuristicSelector(SgProject* project, long defaultCount, long threshold)
		: defaultCount(defaultCount), threshold(threshold) {
	// Initiate CallGraphManager
	cgManager = new CallGraphManager(project, &callsByCaller);
}

/** \brief does nothing yet */
void CallFrequencyHeuristicSelector::selectionBegin(SgProject* project) {}

/** \brief is called on every node of the AST */
void CallFrequencyHeuristicSelector::visit(SgNode* n) {
	if (isSgFunctionDefinition(n)) {
		return;
	}

	SgFunctionCallExp* funcCall = isSgFunctionCallExp(n);
	if (funcCall) {
		callExps.insert(funcCall);
		return;
	}

	SgScopeStatement* scopeStmt = isSgScopeStatement(n);
	if (scopeStmt) {	// is scope

		long count;

		switch (scopeStmt->variantT()) {
			case V_SgForStatement:
				count = approximateCount(isSgForStatement(scopeStmt));
				break;

			case V_SgWhileStmt:
				count = approximateCount(isSgWhileStmt(scopeStmt));
				break;

			case V_SgDoWhileStmt:
				count = approximateCount(isSgDoWhileStmt(scopeStmt));
				break;

			default:
				count = 1L;	// no loop, only simple scope
		}

		loopCounts[scopeStmt] = count;
	}
}

/** \brief is called after the visitor */
void CallFrequencyHeuristicSelector::selectionEnd(SgProject* project) {
#if VERBOSE > 0
	dumpLoopCounts();
	dumpFunctionCalls();
#endif

	// save approximate #calls per function
	boost::unordered_set<SgFunctionCallExp*, long>::iterator itFCE;
	for (itFCE = callExps.begin(); itFCE != callExps.end(); itFCE++) {
		SgScopeStatement* scopeStmt = getEnclosingSgScopeStatement(*itFCE);
		if (scopeStmt == NULL) {
			std::cerr << "scopeStmt was NULL" << std::endl;
			continue;
		}

		long long loopCount = 1L;	// everything is called at least once

		while (!isSgFunctionDefinition(scopeStmt)) {
			//			std::cout << "Caught in While loop" << std::endl;
			loopCount *= loopCounts[scopeStmt];
			scopeStmt = getEnclosingSgScopeStatement(scopeStmt);
		}

		//		scopeStmt = SageInterface::getEnclosingFunctionDefinition(scopeStmt);
		//		if(scopeStmt == NULL){
		//			std::cerr << "scopeStmt was NULL" << std::endl;
		//			continue;
		//		}
		SgFunctionDefinition* calleeFuncDef = ASTHelper::getFunctionDefinition(*itFCE);
		// 2014-03 JP: QuickFix
		if (calleeFuncDef == NULL)
			continue;

		SgFunctionDeclaration* calleeFuncDecl = ASTHelper::getFirstNondefDeclaration(calleeFuncDef);
		if (calleeFuncDecl == NULL) {
			std::cerr << "caleeFuncDecl == NULL" << std::endl;
			continue;
		}
		SgFunctionDeclaration* caller = isSgFunctionDefinition(scopeStmt)->get_declaration();
		if (caller == NULL) {
			std::cerr << "caller == NULL" << std::endl;
			continue;
		}
		caller = ASTHelper::getFirstNondefDeclaration(caller);

#if VERBOSE > 1
		std::cout << loopCount << ": " << ASTHelper::nondefUnparseToString(calleeFuncDecl) << " called by "
							<< ASTHelper::nondefUnparseToString(caller) << std::endl;
#endif

		callsByCaller[calleeFuncDecl][caller] += loopCount;
		callees.insert(calleeFuncDef);
	}

#if VERBOSE > 0
	dumpCallsByCaller();
#endif

	// determine calls by function do the selection
	prepareAndDoSelection();

#if VERBOSE > 0
	dumpNumberOfCalls();
#endif
}

/** \brief determine calls by function, then do the selection */
void CallFrequencyHeuristicSelector::prepareAndDoSelection() {
	boost::unordered_set<SgFunctionDefinition*>::iterator itFD;
	for (itFD = callees.begin(); itFD != callees.end(); itFD++) {
		SgFunctionDeclaration* callee = ASTHelper::getFirstNondefDeclaration(*itFD);

		long long actual = cgManager->approximateNumberOfCalls(callee);
		callsByFuncDef[*itFD] += actual;	// for verbose output

		if (actual < threshold) {
			this->select(*itFD);
		}
	}
}

SgScopeStatement* CallFrequencyHeuristicSelector::getEnclosingSgScopeStatement(SgNode* n) {
	return SageInterface::getEnclosingNode<SgScopeStatement>(n, false);
}

long CallFrequencyHeuristicSelector::approximateCount(SgForStatement* forStmt) {
	SgStatement* forTestStmt = forStmt->get_test();
	if (isSgExprStatement(forTestStmt) == NULL) {
		std::cerr << "Error: Not an expression statement as second statement in loop head." << std::endl;
	}
	if (isSgNullStatement(forTestStmt)) {
		std::cerr << "Found SgNullStatement @ line: ";
		std::cerr << forTestStmt->get_file_info()->get_line() << std::endl;
		return defaultCount;
	}

	// 2014-04 JP: XXX This is a deprecated function and leads to crashing at the SPEC benchmarks,
	//		We should use get_test() instead, which returns a statement.
	// test expression
	//	SgExpression* testExpr = forStmt->get_test_expr();
	SgExpression* testExpr = isSgExprStatement(forTestStmt)->get_expression();

	// only covers < and <= test expressions
	VariantT vT = testExpr->variantT();
	if (vT == V_SgLessThanOp || vT == V_SgLessOrEqualOp) {
		SgBinaryOp* binOp = isSgBinaryOp(forStmt->get_test_expr());
		if (binOp) {
			// assume that rhs of test expression is the size
			SgExpression* expr = binOp->get_rhs_operand();

			return getValue(expr);
		}
	}
	return defaultCount;
}

long CallFrequencyHeuristicSelector::approximateCount(SgWhileStmt* forStmt) {
	// XXX this one is dumb for now
	return defaultCount;
}

long CallFrequencyHeuristicSelector::approximateCount(SgDoWhileStmt* forStmt) {
	// XXX this one is dumb for now
	return defaultCount;
}

long CallFrequencyHeuristicSelector::getValue(SgExpression* expr) {
	if (expr == NULL)
		std::cerr << "ERROR: In " << __FUNCTION__ << " : expr was NULL" << std::endl;

	switch (expr->variantT()) {
		SgInitializedName* iniVarName;

		// the value is a reference on a variable
		case V_SgVarRefExp:
			iniVarName = isSgVarRefExp(expr)->get_symbol()->get_declaration();
			if (iniVarName) {
				SgInitializer* initializer = iniVarName->get_initializer();
				if (!initializer) {
					return defaultCount;
				}
			}

			// XXX RN: 2014-03 Is there an elegant way to do this?
			return getValue(isSgVarRefExp(expr)->get_symbol()->get_declaration()->get_initptr());

		// e.g. x=23
		case V_SgAssignInitializer:
			return getValue(isSgAssignInitializer(expr)->get_operand());

		// cast expression (e.g. 100 -> 100L)
		case V_SgCastExp:
			return getValue(isSgCastExp(expr)->get_operand());

		// actual value expressions
		case V_SgIntVal:
			return (long)isSgIntVal(expr)->get_value();
			break;
		case V_SgUnsignedIntVal:
			return (long)isSgUnsignedIntVal(expr)->get_value();
			break;

		case V_SgCharVal:
			return (long)isSgCharVal(expr)->get_value();
			break;
		case V_SgUnsignedCharVal:
			return (long)isSgUnsignedCharVal(expr)->get_value();
			break;

		case V_SgShortVal:
			return (long)isSgShortVal(expr)->get_value();
			break;
		case V_SgUnsignedShortVal:
			return (long)isSgUnsignedShortVal(expr)->get_value();
			break;

		case V_SgLongIntVal:
			return (long)isSgLongIntVal(expr)->get_value();
			break;
		case V_SgUnsignedLongVal:
			return (long)isSgUnsignedLongVal(expr)->get_value();
			break;

		default:
			return defaultCount;
	}
}

// ===== FOR VERBOSE OUTPUT =====
#if VERBOSE

void CallFrequencyHeuristicSelector::dumpLoopCounts() {
	std::cout << "=====Dumping heuristical Loop Counts=====" << std::endl;

	boost::unordered_map<SgScopeStatement*, long>::iterator it;
	for (it = loopCounts.begin(); it != loopCounts.end(); it++) {
		SgScopeStatement* scopeStmt = (*it).first;
		Sg_File_Info* fileInfo = scopeStmt->get_file_info();

		int line = fileInfo->get_physical_line();

		std::cout << (*it).second << "\t" << ((*it).first)->class_name() << "\t" << fileInfo->get_filename() << " : line "
							<< line << std::endl;
	}

	std::cout << std::endl;
}

void CallFrequencyHeuristicSelector::dumpFunctionCalls() {
	std::cout << "=====Dumping registered Function Call Expressions=====" << std::endl;

	boost::unordered_set<SgFunctionCallExp*>::iterator it;
	for (it = callExps.begin(); it != callExps.end(); it++) {
		std::cout << (*it)->getAssociatedFunctionDeclaration()->unparseToString() << std::endl;
	}

	std::cout << std::endl;
}

void CallFrequencyHeuristicSelector::dumpCallsByCaller() {
	std::cout << "=====Dumping Callees to Calls (by Callers)=====" << std::endl;

	for (CalleeToCallerMap::iterator it = callsByCaller.begin(); it != callsByCaller.end(); it++) {
		SgFunctionDeclaration* callee = it->first;
		CallerMap callsByFunc = it->second;

		std::cout << callee->unparseToString() << " called by " << std::endl;

		for (CallerMap::iterator itC = callsByFunc.begin(); itC != callsByFunc.end(); itC++) {
			SgFunctionDeclaration* caller = itC->first;
			long long count = itC->second;

			std::cout << "\t" << count << "\t: " << caller->unparseToString() << std::endl;
		}
	}
	std::cout << std::endl;
}

void CallFrequencyHeuristicSelector::dumpNumberOfCalls() {
	std::cout << "=====Dumping Calls by Functions=====" << std::endl;

	boost::unordered_map<SgFunctionDefinition*, long long>::iterator it;
	for (it = callsByFuncDef.begin(); it != callsByFuncDef.end(); it++) {
		SgFunctionDeclaration* funcDecl = it->first->get_declaration();
		funcDecl = ASTHelper::getFirstNondefDeclaration(funcDecl);	// avoid unparsing body

		std::cout << it->second << "\t" << funcDecl->unparseToString() << std::endl;
	}

	std::cout << std::endl;
}

#endif
