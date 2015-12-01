#ifndef INSTRO_ROSE_UTILITY_ASTHELPER_H
#define INSTRO_ROSE_UTILITY_ASTHELPER_H

#include "rose.h"

#include <string>

namespace InstRO {
namespace Rose {
namespace Utility {

/**
 * \brief methods to simplify handling of AST nodes
 * \author Roman
 */
namespace ASTHelper {
	/* Return the defining declaration for a given declaration */
	SgFunctionDeclaration* getDefiningDeclaration(SgFunctionDeclaration* oldDecl);
	/* Return the first non-defining declaration for a given declaration */
	SgFunctionDeclaration* getFirstNondefDeclaration(SgFunctionDeclaration* oldDecl);

	/* Return the defining declaration for a given definition */
	SgFunctionDeclaration* getDefiningDeclaration(SgFunctionDefinition* oldDef);
	/* Return the first non-defining declaration for a given definition */
	SgFunctionDeclaration* getFirstNondefDeclaration(SgFunctionDefinition* oldDef);

	/* Return the definition for a given function call expression */
	SgFunctionDefinition* getFunctionDefinition(SgFunctionCallExp* callExp);

	/* Return the loop nesting level of a statement */
	int getLoopNestingLevel(SgLocatedNode* n);

	std::string nondefUnparseToString(SgFunctionDeclaration* funcDecl);
	std::string nondefUnparseToString(SgFunctionDefinition* funcDef);

	std::string getFunctionSignature(SgFunctionDefinition* funcDef);

	/* private helper functions for loop-nesting level */
	SgScopeStatement* getEnclosingLoop(SgLocatedNode* n);
	bool isLoop(SgScopeStatement* scope);
}

}	// namespace Utility
}	// namespace Rose
}	// namespace InstRO

#endif	// ASTHELPER_H
