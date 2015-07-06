#include "rose.h"

#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
#include <string>
#include <iostream> // The development version uses cout for debugging...
#include "instro/core/ConstructSet.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/rose/tooling/RoseNamedConstructAccess.h"
namespace InstRO {
namespace Rose {
namespace Tooling {
namespace NamedConstructAccess {

/*CI: The intend here is to match the smallest possible fragment that still qualifies.
	If a user wants to have a "higher" construct, he can use construct elevation to get the next higher element
*/
bool NameMatchingASTTraversal::relevantNode(SgNode* n)
{
	return false;
}
void NameMatchingASTTraversal::select(SgNode * n)
{
	// get the closes representative for n as a construct and leave
	if (isSgFunctionDefinition(n))
        {
		csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(n));
	}
	else if (isSgVarRefExp(n))
	{
	}	
}
void NameMatchingASTTraversal::preOrderVisit(SgNode* n) {
	/*
	   if (this->matchingObject == NULL || this->listToMatchAgainst == NULL) {
	   std::cerr << "Matching object and/or listToMatchAgainst are NULL. Please use init(...) before invoking the traversal."
	   << std::endl;
	   throw std::string("NameMatchingSelector was not initialized before starting traversal.");
	   }
	 */
	std::string stringToMatch;
	bool performMatch=false;
	if (isSgFunctionDefinition(n))
	{
		stringToMatch=isSgFunctionDefinition(n)->get_declaration()->get_name();
		performMatch=true;
	}
	else if (isSgVarRefExp(n))
	{	
		stringToMatch=isSgVarRefExp(n)->get_symbol()->get_name();
		performMatch=true;
	}
	if (false)
	{
		std::string generatedName = toString(n);
		if (generatedName.compare("INSTRUMENTOR_NOT_SUPPORTED_ATM") != 0) {
		}
	}
	if (performMatch){
		std::cout << "Found an Identifyer ... matching" << std::endl;
		// generate a string from the rose AST node ...
		// Check the generated name against the stored list
		/* 2013-10-15 JP: This was based on a previous implementation in the matcher.
		   lastMatchIds = this->matchingObject->match(generatedName, *listToMatchAgainst);
		   if (lastMatchIds.size() > 0) {
		 */

		if (this->matchingObject->isMatch(stringToMatch)) {
			//			if (verbose) {
			std::cout << "Matcher tried to match: " << stringToMatch << " and returned " << true
				<< ". Leading to node selection." << std::endl;
			//			}

			// XXX RN 2014-10: this dirty hack is necessary because for MPI methods we only find the SgFunctionRefExp
			// XXX RN is this a common behavior for handling SgFunctionRefExp?
			/*			SgFunctionRefExp* fRefExp = isSgFunctionRefExp(n);
						if (fRefExp) {
						SgFunctionDeclaration* fDecl = fRefExp->getAssociatedFunctionDeclaration();
						if (!fDecl) {
						std::cout << "NMSelector: Declaration of a SgFunctionRefExp was null." << std::endl;
						}
						select(fDecl);
						return;*/
			//			}	
			this->select(n);
		}
	}
	
}
void NameMatchingASTTraversal::postOrderVisit(SgNode* n) {
	if (preOrderMatch) {
		if (n == nodeTracker.top())
			nodeTracker.pop();
	} else if (postOrderMatch) {
	}
}
std::string NameMatchingASTTraversal::toString(SgNode* n) {
	// generates a string representation of n
	if (isSgFunctionDefinition(n) != NULL) {
		// This is a function definition node which has a name
		return isSgFunctionDefinition(n)->get_declaration()->get_qualified_name().getString();
	} else if (isSgFunctionRefExp(n) != NULL) {
		// This is a function reference node which has a name
		return isSgFunctionRefExp(n)->getAssociatedFunctionDeclaration()->get_qualified_name().getString();
	} else if (isSgMemberFunctionRefExp(n) != NULL) {
		// This is a member function reference node which has a name
		return isSgMemberFunctionRefExp(n)->getAssociatedMemberFunctionDeclaration()->get_qualified_name().getString();
	}
	/* JPL: At the moment it is not sensible to give the opportunity to mark single variables.
	 else if(isSgVarRefExp(n) != NULL){
	 // This is a variable reference node which has a name
	 return isSgVarRefExp(n)->get_symbol()->get_name().getString();
	 } else if(isSgVariableDefinition(n) != NULL){
	 // This is a variable definition which has a name
	 return isSgVariableDefinition(n)->get_vardefn()->get_name().getString();
	 }
	 */
	// If we reach this statement we know, that this node is not supported.
	return std::string("INSTRUMENTOR_NOT_SUPPORTED_ATM");
}
}
}
}
}	// Cose the namespaces
