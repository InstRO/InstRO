#include "instro/utility/asthelper.h"


using namespace InstRO;

/**
 * \brief Shortcut to get the defining declaration of a given declaration
 * \param oldDecl a possibly non-defining declaration
 */
SgFunctionDeclaration* ASTHelper::getDefiningDeclaration(SgFunctionDeclaration* oldDecl) {
	SgDeclarationStatement* definingDecl = oldDecl->get_definingDeclaration();

	return isSgFunctionDeclaration(definingDecl);
}

/**
 * \brief Shortcut to get the first non-defining declaration of a given declaration
 * \param oldDecl a possibly defining declaration
 */
SgFunctionDeclaration* ASTHelper::getFirstNondefDeclaration(SgFunctionDeclaration* oldDecl) {
	SgDeclarationStatement* nondefDecl = oldDecl->get_firstNondefiningDeclaration();

	return isSgFunctionDeclaration(nondefDecl);
}

SgFunctionDeclaration* ASTHelper::getDefiningDeclaration(SgFunctionDefinition* oldDef) {
	SgFunctionDeclaration* funcDecl = oldDef->get_declaration();

	return getDefiningDeclaration(funcDecl);
}

SgFunctionDeclaration* ASTHelper::getFirstNondefDeclaration(SgFunctionDefinition* oldDef) {
	SgFunctionDeclaration* funcDecl = oldDef->get_declaration();

	return getFirstNondefDeclaration(funcDecl);
}

/**
 * \brief Shortcut to get the definition corresponding to a function call expression.
 * \param callExp The function call expression aka. the function call
 *
 * 	XXX RN 2014-02: can the declaration always be resolved?
 *	XXX JP 2014-03: No. If it is a template function the call to getAssocitatedFunctionDeclaration() will return NULL
 *			It might be possible to get around this using the symbol table but I cant remember at the moment.
 */
SgFunctionDefinition* ASTHelper::getFunctionDefinition(SgFunctionCallExp* callExp) {
	SgFunctionDeclaration* funcDecl = callExp->getAssociatedFunctionDeclaration();

	if(!funcDecl) {
		std::cout << "Cannot get associated declaration of function call: "
				<< callExp->unparseToString() << std::endl;
		return NULL;
	}

	SgFunctionDeclaration* fDefDecl = ASTHelper::getDefiningDeclaration(funcDecl);
	if(fDefDecl == NULL)
		return NULL;
	SgFunctionDefinition* funcDef = fDefDecl->get_definition();

	if(!funcDef) {
		std::cout << "Cannot get FuncDef of declaration:"
				<< funcDecl->unparseToString() << std::endl;
	}

	return funcDef;
}

/**
 * \brief Returns the loop nesting level of a statement
 * \param n A node of the AST
 */
int ASTHelper::getLoopNestingLevel(SgLocatedNode* n) {
	int nestingCount=0;
	SgScopeStatement* enclosingLoop = getEnclosingLoop(n);

	while(enclosingLoop) {
		nestingCount++;
		enclosingLoop = getEnclosingLoop(enclosingLoop);
	}
	return nestingCount;
}

std::string ASTHelper::nondefUnparseToString(SgFunctionDeclaration* funcDecl) {
	return ASTHelper::getFirstNondefDeclaration(funcDecl)->unparseToString();
}

std::string ASTHelper::nondefUnparseToString(SgFunctionDefinition* funcDef) {
	return ASTHelper::nondefUnparseToString(funcDef->get_declaration());
}

/**
 * \brief Returns enclosing loop of a node
 * \param n A node of the AST
 */
SgScopeStatement* ASTHelper::getEnclosingLoop(SgLocatedNode* n) {
	SgScopeStatement* enclosingScope = SageInterface::getEnclosingNode<SgScopeStatement>(n,false);

	if(isLoop(enclosingScope)) {
		return enclosingScope;

	} else if(isSgGlobal(enclosingScope)){
		return NULL;	// reached global scope

	} else {
		return getEnclosingLoop(enclosingScope);
	}
}

/**
 * \brief Returns true if a node is a loop. \
 * Possible loop statements in C++ are: For, While, Do-While
 * \param n A node of the AST
 */
bool ASTHelper::isLoop(SgScopeStatement* scope) {
	return isSgForStatement(scope) || isSgWhileStmt(scope) || isSgDoWhileStmt(scope);
}

/** \brief Returns the corresponding function signature like "foo(int,float*)" */
std::string ASTHelper::getFunctionSignature(SgFunctionDefinition* funcDef) {

	if (funcDef == NULL) {
		std::cout << "getFunctionSignature: funcDef was null" << std::endl;
		return "null";
	}

	SgFunctionDeclaration* funcDecl = funcDef->get_declaration();

	SgInitializedNamePtrList params = funcDecl->get_parameterList()->get_args();

	std::stringstream ss;
	ss << funcDecl->get_qualified_name().getString() << "(";

	for (int i=0; i<params.size(); i++) {
		if (i!=0) {
			ss << ",";
		}
		ss << params[i]->get_type()->unparseToString();
	}
	ss << ")";

	return ss.str();
}
