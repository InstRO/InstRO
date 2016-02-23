#include "instro/rose/tooling/RoseNamedConstructAccess.h"
#include "instro/rose/core/RoseConstructSet.h"

#include "instro/utility/Logger.h"
#define INSTRO_LOG_LEVEL DEBUG

#include <string>

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace NamedConstructAccess {

InstRO::Core::ConstructSet NameMatchingASTTraversal::getResultingCS() { return cs; }

void NameMatchingASTTraversal::visit(SgFunctionDefinition* n) {
	auto candidate = n->get_declaration()->get_qualified_name().getString();
	logIt(DEBUG) << "Visiting FunctionDefinition for qualified_name: " << candidate << std::endl;
	if (matchingObject->isMatch(candidate)) {
		logIt(DEBUG) << "Matched pattern." << std::endl;
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(n));
	}
}
void NameMatchingASTTraversal::visit(SgVarRefExp* n) {
	auto candidate = n->get_symbol()->get_name();
	if (matchingObject->isMatch(candidate)) {
		// insert all enclosing expressions until you encounter the first enclosing statement
		SgNode* current = n;
		while (current != nullptr && !isSgStatement(current)) {
			csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(current));
			current = current->get_parent();
		}
	}
}
void NameMatchingASTTraversal::visit(SgFunctionRefExp* n) {
	handleFunctionRef(n->getAssociatedFunctionDeclaration(), n);
}
void NameMatchingASTTraversal::visit(SgTemplateFunctionRefExp* n) {
	handleFunctionRef(n->getAssociatedFunctionDeclaration(), n);
}
void NameMatchingASTTraversal::visit(SgMemberFunctionRefExp* n) {
	logIt(DEBUG) << "Visiting MemberFunctionRefExp" << std::endl;
	handleFunctionRef(n->getAssociatedMemberFunctionDeclaration(), n);
}
void NameMatchingASTTraversal::visit(SgTemplateMemberFunctionRefExp* n) {
	handleFunctionRef(n->getAssociatedMemberFunctionDeclaration(), n);
}
void NameMatchingASTTraversal::visit(SgVariableDeclaration* n) {
	if (InstRO::Rose::Core::RoseConstructTraitPredicates::DefinedVariableDeclarationPredicate()(n)) {
		auto initializedName = n->get_variables()[0];
		auto candidate = initializedName->get_qualified_name().getString();

		if (matchingObject->isMatch(candidate)) {
			csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(n));
		}
	}
}
void NameMatchingASTTraversal::visit(SgNode* n) {
	// default case: nothing to do here
}

void NameMatchingASTTraversal::handleFunctionRef(SgFunctionDeclaration* associatedDecl, SgExpression* funcRef) {
	auto candidate = associatedDecl->get_qualified_name().getString();
	logIt(DEBUG) << "Handling FunctionDeclaration for call with qualified_name: " << candidate << std::endl;
	if (matchingObject->isMatch(candidate)) {
			logIt(DEBUG) << "Matched pattern." << std::endl;
			auto parent = funcRef->get_parent();
			if(isSgDotExp(parent) || isSgArrowExp(parent)){
				parent = parent->get_parent();
			}
			csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(parent));
	}
}

}	// namespace NamedConstructAccess
}	// namespace Tooling
}	// namespace Rose
}	// namespace InstRO
