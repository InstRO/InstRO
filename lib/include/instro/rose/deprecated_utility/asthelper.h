#ifndef ASTHELPER_H
#define ASTHELPER_H

#include "rose.h"

#include <string>

namespace InstRO {

/**
 * \brief This is an abstract final class that contains static methods to simplify handling of AST nodes
 * \author Roman
 */
class ASTHelper {
	// XXX 2013-10-08 JP: Is this class necessary? \
// XXX 2013-12 RN: How else to hide private methods, without a class?
	// XXX 2014-03 JP: I don't know if they need to be private, maybe use to some kind of utility namespace?
 public:
	/* Return the defining declaration for a given declaration */
	static SgFunctionDeclaration* getDefiningDeclaration(SgFunctionDeclaration* oldDecl);
	/* Return the first non-defining declaration for a given declaration */
	static SgFunctionDeclaration* getFirstNondefDeclaration(SgFunctionDeclaration* oldDecl);

	/* Return the defining declaration for a given definition */
	static SgFunctionDeclaration* getDefiningDeclaration(SgFunctionDefinition* oldDef);
	/* Return the first non-defining declaration for a given definition */
	static SgFunctionDeclaration* getFirstNondefDeclaration(SgFunctionDefinition* oldDef);

	/* Return the definition for a given function call expression */
	static SgFunctionDefinition* getFunctionDefinition(SgFunctionCallExp* callExp);

	/* Return the loop nesting level of a statement */
	int getLoopNestingLevel(SgLocatedNode* n);

	static std::string nondefUnparseToString(SgFunctionDeclaration* funcDecl);
	static std::string nondefUnparseToString(SgFunctionDefinition* funcDef);

	static std::string getFunctionSignature(SgFunctionDefinition* funcDef);

 private:
	/* no other class can now derive from this class */
	virtual ~ASTHelper() = 0;

	/* private helper functions for loop-nesting level */
	SgScopeStatement* getEnclosingLoop(SgLocatedNode* n);
	bool isLoop(SgScopeStatement* scope);
};
}

#endif	// ASTHELPER_H
