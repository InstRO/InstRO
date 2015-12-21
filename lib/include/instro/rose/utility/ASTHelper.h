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
 * \author Jan
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

bool isContainedInTemplateInstantiation(SgNode* node);

/* Handles SgTemplateFunctionInstantiationDecl and SgTemplateMemberFunctionInstantiationDecl to dispatch the Consumer
 * accordingly */
template <typename CallableFileInfoConsumer>
typename std::result_of<CallableFileInfoConsumer(Sg_File_Info*)>::type applyConsumerToTemplateInstantiationDecl(
		CallableFileInfoConsumer cr, SgNode *node) {
	SgTemplateInstantiationFunctionDecl *funcDecl =
			SageInterface::getEnclosingNode<SgTemplateInstantiationFunctionDecl>(node, true);
	SgTemplateInstantiationMemberFunctionDecl *memDecl =
			SageInterface::getEnclosingNode<SgTemplateInstantiationMemberFunctionDecl>(node, true);

	typename std::result_of<CallableFileInfoConsumer(Sg_File_Info*)>::type retVal;

	if (funcDecl != nullptr) {
		// FIXME It seems ROSE is unable to resolve the template in this situation
		auto templDecl = funcDecl->get_templateDeclaration();
		retVal = cr(templDecl->get_startOfConstruct());
	}
	if (memDecl != nullptr) {
		auto templDecl = memDecl->get_templateDeclaration();
		retVal = cr(templDecl->get_startOfConstruct());
	}

	return retVal;
}
}

}	// namespace Utility
}	// namespace Rose
}	// namespace InstRO

#endif	// ASTHELPER_H
