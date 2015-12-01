#include <string>

#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/tooling/RoseNamedConstructAccess.h"
#include "instro/utility/Logger.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace NamedConstructAccess {

InstRO::Core::ConstructSet NameMatchingASTTraversal::getResultingCS() {
		return cs;
	}

void NameMatchingASTTraversal::visit(SgFunctionDefinition* n) {
	auto candidate = n->get_declaration()->get_qualified_name().getString();
	if (matchingObject->isMatch(candidate)) {
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(n));
	}
}
void NameMatchingASTTraversal::visit(SgVarRefExp* n) {
	auto candidate = n->get_symbol()->get_name();
	if (matchingObject->isMatch(candidate)) {
		SgNode* current = n;
		while (current!=nullptr && !isSgStatement(current)) {
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
	handleFunctionRef(n->getAssociatedMemberFunctionDeclaration(), n);
}
void NameMatchingASTTraversal::visit(SgTemplateMemberFunctionRefExp* n) {
	handleFunctionRef(n->getAssociatedMemberFunctionDeclaration(), n);
}
void NameMatchingASTTraversal::visit(SgNode* n) {
	// default case: nothing to do here
}

void NameMatchingASTTraversal::handleFunctionRef(SgFunctionDeclaration* associatedDecl, SgExpression* funcRef) {
	auto candidate = associatedDecl->get_qualified_name().getString();
		if (matchingObject->isMatch(candidate)) {
			csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(funcRef->get_parent()));
		}
}

}
}
}
}	// Cose the namespaces
