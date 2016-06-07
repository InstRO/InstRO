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
 * \author JP Lehr
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

/** returns true if a void-returning function does not end with a return statement */
bool voidFunctionEndsWithoutReturn(SgFunctionDefinition *fDef);

bool isConstructor(SgFunctionDefinition* fDef);
bool isConstructor(SgFunctionDeclaration* fDecl);

bool isDestructor(SgFunctionDefinition* fDef);
bool isDestructor(SgFunctionDeclaration* fDecl);

/* Handles SgTemplateFunctionInstantiationDecl and SgTemplateMemberFunctionInstantiationDecl to dispatch the Consumer
 * accordingly */
template <typename CallableFileInfoConsumer>
typename std::result_of<CallableFileInfoConsumer(Sg_File_Info*)>::type applyConsumerToTemplateInstantiationDecl(
		CallableFileInfoConsumer cr, SgNode* node) {
	SgTemplateInstantiationFunctionDecl* funcDecl =
			SageInterface::getEnclosingNode<SgTemplateInstantiationFunctionDecl>(node, true);
	SgTemplateInstantiationMemberFunctionDecl* memDecl =
			SageInterface::getEnclosingNode<SgTemplateInstantiationMemberFunctionDecl>(node, true);

	typename std::result_of<CallableFileInfoConsumer(Sg_File_Info*)>::type retVal;

	if (funcDecl != nullptr) {
		// FIXME It seems ROSE is unable to resolve the template in this situation
		auto templDecl = funcDecl->get_templateDeclaration();
		templDecl = isSgTemplateFunctionDeclaration(templDecl->get_definingDeclaration());
		retVal = cr(templDecl->get_startOfConstruct());
	}

	if (memDecl != nullptr) {
		auto templDecl = isSgTemplateMemberFunctionDeclaration(memDecl->get_templateDeclaration());
		if (memDecl->isCompilerGenerated()) {
			/* Since ROSE handles templates _very_ inconsistently we use the check on the file id to guide
			 * which Sg_File_Info object we want to use.
			 * -2 indicates a "compiler generated" filename
			 * -4 indicates a "NULL FILE" filename
			 */
			if ((templDecl->get_startOfConstruct()->get_file_id() == -2) ||
					(templDecl->get_startOfConstruct()->get_file_id() == -4)) {
				templDecl = isSgTemplateMemberFunctionDeclaration(templDecl->get_definingDeclaration());
			}
		}
		retVal = cr(templDecl->get_startOfConstruct());
	}

	return retVal;
}

template <typename CallableFileInfoConsumer>
typename std::result_of<CallableFileInfoConsumer(SgInitializedName*)>::type applyConsumerToAssignInitializer(
		CallableFileInfoConsumer cr, SgAssignInitializer* node) {
	auto decl = isSgDeclarationStatement(node->get_parent()->get_parent());
	auto initName = isSgInitializedName(node->get_parent());

	if (decl != nullptr) {
		// handle the case within an if statement
		if (decl->isCompilerGenerated()) {
			auto n = decl->get_parent();
			auto ln = isSgLocatedNode(n);
			if (ln == nullptr) {
				throw std::string("The Assign initializer was in a weird position!");
			}
			return cr(initName);
		}

		// handle the "normal" assign initializer
		return cr(initName);
	}
	throw std::string("The Assign initializer was in a weird position!");
}

/** Given a node it returns the file info if this node is either an if or a while. Returns nullptr otherwise */
Sg_File_Info* getFileInfoFromWhileOrIf(SgNode* n);
}

}	// namespace Utility
}	// namespace Rose
}	// namespace InstRO

#endif	// ASTHELPER_H
