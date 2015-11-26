#include "rose.h"

#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
#include <string>
#include <iostream>	// The development version uses cout for debugging...
#include "instro/core/ConstructSet.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/tooling/RoseNamedConstructAccess.h"
#include "instro/utility/Logger.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace NamedConstructAccess {

/*CI: The intend here is to match the smallest possible fragment that still qualifies.
	If a user wants to have a "higher" construct, he can use construct elevation to get the next higher element
*/
bool NameMatchingASTTraversal::relevantNode(SgNode* n) { return false; }

void NameMatchingASTTraversal::select(SgNode* n) {
	// if the node is directly an instrumentable node ...
	if (isSgFunctionDefinition(n) || isSgVarRefExp(n) || isSgFunctionRefExp(n) || isSgMemberFunctionRefExp(n)) {
		// get the corresponding constructset (roseProvider) and put it to the building output construct set
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(n));
	}
}

void NameMatchingASTTraversal::preOrderVisit(SgNode* n) {
	
	std::string stringToMatch = generateStringToMatch(n);

	// If we found a node with the right text, and the text is not zero
	if (stringToMatch.length() > 0) {
		logIt(DEBUG) << "NameMatchingASTTraversal::preOrderVisit:\t >" << stringToMatch << "<" << std::endl;

		if (this->matchingObject->isMatch(stringToMatch)) {
			logIt(DEBUG) << "NameMatchingASTTraversal::preOrderVisit:\t found match" << std::endl;
			this->select(n);
		}
	}
}

void NameMatchingASTTraversal::postOrderVisit(SgNode* n) {
	if (preOrderMatch) {
		if (n == nodeTracker.top())
			nodeTracker.pop();
	}
}

std::string NameMatchingASTTraversal::generateStringToMatch(SgNode* n) {
	// generates a string representation of n
	if (isSgFunctionDefinition(n)) {
		return isSgFunctionDefinition(n)->get_declaration()->get_qualified_name().getString();
	} else if (isSgFunctionRefExp(n)) {
		return isSgFunctionRefExp(n)->getAssociatedFunctionDeclaration()->get_qualified_name().getString();
	} else if (isSgMemberFunctionRefExp(n)) {
		return isSgMemberFunctionRefExp(n)->getAssociatedMemberFunctionDeclaration()->get_qualified_name().getString();
	} else if (isSgVarRefExp(n)) {
		return isSgVarRefExp(n)->get_symbol()->get_name();
	}

	// If we reach this statement we know, that this node is not supported.
	return std::string("");
}
}
}
}
}	// Cose the namespaces
