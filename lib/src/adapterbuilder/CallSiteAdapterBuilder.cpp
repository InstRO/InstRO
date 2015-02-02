
#include "instro/adapterbuilder/CallSiteAdapterBuilder.h"

#include <functional>
#include <iostream> //XXX

#include <sstream>

const std::string CallSiteAdapterBuilder::FUNCTION_NAME_ENTER = "__cyg_profile_func_enter";
const std::string CallSiteAdapterBuilder::FUNCTION_NAME_EXIT  = "__cyg_profile_func_exit";

CallSiteAdapterBuilder::CallSiteAdapterBuilder(Selector* decidingSelector) :
		AdapterBuilder(decidingSelector),
		instrumentationDeclared(false) {
}

void CallSiteAdapterBuilder::buildForNode(SgNode* node) {

	SgFunctionCallExp* callExp = isSgFunctionCallExp(node);
	if (callExp) {

		bool transformation = transformationNecessary(callExp);

		///XXX
		SgType* type = callExp->get_type();
		std::cout << " # funcCallExp: " << callExp->unparseToString()
				<< " | transformation: " << transformation
				<< " | type: " << SageInterface::get_name(type)
				<< std::endl;

		if (transformation) {
			doTransformation(callExp);
		}
		callExprsToInstrument.insert(callExp);
	}
}

/** Instrument all previously found function calls */
void CallSiteAdapterBuilder::modificationEnd() {
	for (SgFunctionCallExp* callExp : callExprsToInstrument) {

		if (SageInterface::getScope(callExp) == SageInterface::getGlobalScope(callExp)) {
			continue;	// XXX skip function calls in the global scope for now
		}

		SgStatement* enclosingStmt = SageInterface::getEnclosingStatement(callExp);
		encloseWithInstrumentation(enclosingStmt, callExp);
	}
}

/** Returns true if the call's enclosing statement has to be transformed before instrumentation */
bool CallSiteAdapterBuilder::transformationNecessary(SgFunctionCallExp* callExp) {
	// TODO: call in the test of an if expression has to be handled

	SgStatement* enclosingStatement = SageInterface::getEnclosingStatement(callExp);

	Rose_STL_Container<SgFunctionCallExp*> callsOfEnclosing =
			SageInterface::querySubTree<SgFunctionCallExp>(enclosingStatement, V_SgFunctionCallExp);

	Rose_STL_Container<SgFunctionCallExp*> callsWithinCallExp =
			SageInterface::querySubTree<SgFunctionCallExp>(callExp, V_SgFunctionCallExp);

	// transform if multiple calls within the statement, but not all are within the call (e.g. as parameters)
	return callsOfEnclosing.size() > 1 && (callsWithinCallExp.size() < callsOfEnclosing.size());
}

void CallSiteAdapterBuilder::doTransformation(SgFunctionCallExp* callExp) {

	// TODO: handle void type

	SgFunctionCallExp* callExpCopy = SageInterface::deepCopy(callExp);


	std::string tmpName = getTmpVarName(callExp);
	SgType* returnType = callExp->get_type();
	SgAssignInitializer* assignInit = SageBuilder::buildAssignInitializer(callExpCopy, returnType);
	SgScopeStatement* scopeStmt = SageInterface::getScope(callExp);

	SgVariableDeclaration* varDecl =
			SageBuilder::buildVariableDeclaration(tmpName, returnType, assignInit, scopeStmt);
	SgStatement* enclosingStmt = SageInterface::getEnclosingStatement(callExp);

	SageInterface::insertStatementBefore(enclosingStmt, varDecl);

	SgVarRefExp* varRef = SageBuilder::buildVarRefExp(varDecl);
	SageInterface::replaceExpression(callExp, varRef, true);
	SageInterface::replaceExpression(callExpCopy, callExp, false);
}


void CallSiteAdapterBuilder::encloseWithInstrumentation(SgStatement* statement, SgFunctionCallExp* callExp) {

	declareInstrumentationFunctions(statement);

	// TODO: for astar calleeFuncDef is sometimes null (e.g. "sqrt(x)" )

	SgFunctionDefinition* calleeFuncDef = ASTHelper::getFunctionDefinition(callExp);
	///XXX
	if (calleeFuncDef == NULL) {
		std::cout << "encloseWithInstrumentation: calleeFuncDef was null | "
				<< " callExp: " << callExp->unparseToString() << std::endl;
	}

	SgFunctionDefinition* callSiteFuncDef = SageInterface::getEnclosingFunctionDefinition(statement);
	///XXX
	if (callSiteFuncDef == NULL) {
		std::cout << "encloseWithInstrumentation: callSiteFuncDef was null" << std::endl;
	}

	SgExprStatement* entryCall =
			createInstrumentationFunction(statement, FUNCTION_NAME_ENTER, calleeFuncDef, callSiteFuncDef);
	SgExprStatement* exitCall =
			createInstrumentationFunction(statement, FUNCTION_NAME_EXIT, calleeFuncDef, callSiteFuncDef);

	SageInterface::insertStatementBefore(statement, entryCall);
	SageInterface::insertStatementAfter(statement, exitCall);
}

SgExprStatement* CallSiteAdapterBuilder::createInstrumentationFunction(SgStatement* statement,
		std::string funcName, SgFunctionDefinition* calleeFuncDef, SgFunctionDefinition* callSiteFuncDef) {

	SgExprListExp* argument_expression_list_enter = SageBuilder::buildExprListExp(
			buildInstrumentationArgument(calleeFuncDef),
			buildInstrumentationArgument(callSiteFuncDef));

	return SageBuilder::buildFunctionCallStmt(
			funcName,
			SageBuilder::buildVoidType(),
			argument_expression_list_enter,
			SageInterface::getGlobalScope(statement));

}

SgExpression* CallSiteAdapterBuilder::buildInstrumentationArgument(SgFunctionDefinition* funcDef) {

	std::string methodSignature = ASTHelper::getFunctionSignature(funcDef);

	///XXX
	std::cout << "  # " << methodSignature << std::endl;

	std::size_t nameHash = std::hash<std::string>()(methodSignature);

	return SageBuilder::buildCastExp(
			SageBuilder::buildUnsignedLongVal((unsigned long) nameHash),
			SageBuilder::buildPointerType(SageBuilder::buildVoidType()));
}


void CallSiteAdapterBuilder::declareInstrumentationFunctions(SgStatement* statement) {
	if (!instrumentationDeclared) {
		buildInstrumentationFunction(SageInterface::getGlobalScope(statement), FUNCTION_NAME_ENTER);
		buildInstrumentationFunction(SageInterface::getGlobalScope(statement), FUNCTION_NAME_EXIT);

		instrumentationDeclared = true;
	}
}

void CallSiteAdapterBuilder::buildInstrumentationFunction(SgScopeStatement* scope, std::string funcName) {

	SgInitializedName* calleeName = SageBuilder::buildInitializedName(
			"callee", SageBuilder::buildPointerType(SageBuilder::buildVoidType()));
	SgInitializedName* callSiteName = SageBuilder::buildInitializedName(
			"callSite", SageBuilder::buildPointerType(SageBuilder::buildVoidType()));

	SgFunctionParameterList* parameterList =
			SageBuilder::buildFunctionParameterList(calleeName, callSiteName);

	SgFunctionDeclaration* funcDecl = SageBuilder::buildNondefiningFunctionDeclaration(
			funcName, SageBuilder::buildVoidType(), parameterList, scope);

	if (SageInterface::getEnclosingNode<SgProject>(scope)->get_Cxx_only()) {
		isSgDeclarationStatement(funcDecl)->set_linkage("C");	// prepend extern "C"
	}
	SageInterface::setExtern(funcDecl);

	scope->prepend_statement(funcDecl);
}


std::string CallSiteAdapterBuilder::getTmpVarName(SgFunctionCallExp* callExp) {

	std::string tmpName = "__tmp";
	std::stringstream ss;
	ss << callExp;
	tmpName += ss.str();

	return tmpName;
}
