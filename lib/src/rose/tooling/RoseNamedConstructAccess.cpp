#include "rose.h"

#include <memory> // We need shared pointers
#include <list> // We use List in the GrammarInterface
#include <string>
#include "instro/core/ConstructSet.h"
#include "instro/rose/tooling/RoseNamedConstructAccess.h"
namespace InstRO{ namespace Rose {	namespace Tooling	{	namespace NamedConstructAccess {

/*CI: The intend here is to match the smallest possible fragment that still qualifies.
  If a user wants to have a "higher" construct, he can use construct elevation to get the next higher element
*/

void NameMatchingASTTraversal::preOrderVisit(SgNode* n) {
	if (preOrderMatch){
		/*
	if (this->matchingObject == NULL || this->listToMatchAgainst == NULL) {
	std::cerr << "Matching object and/or listToMatchAgainst are NULL. Please use init(...) before invoking the traversal." << std::endl;
	throw std::string("NameMatchingSelector was not initialized before starting traversal.");
	}
	*/

		// generate a string from the rose AST node ...
		std::string generatedName = toString(n);
		if (generatedName.compare("INSTRUMENTOR_NOT_SUPPORTED_ATM") != 0)
		{
			// Check the generated name against the stored list
			/* 2013-10-15 JP: This was based on a previous implementation in the matcher.
			lastMatchIds = this->matchingObject->match(generatedName, *listToMatchAgainst);
			if (lastMatchIds.size() > 0) {
			*/

			if (this->matchingObject->isMatch(generatedName)) {
				if (verbose) {
					std::cout << "Matcher tried to match: " << generatedName << " and returned "
						<< true << ". Leading to node selection." << std::endl;

				}

				// XXX RN 2014-10: this dirty hack is necessary because for MPI methods we only find the SgFunctionRefExp
				// XXX RN is this a common behavior for handling SgFunctionRefExp?
				SgFunctionRefExp* fRefExp = isSgFunctionRefExp(n);
				if (fRefExp) {
					SgFunctionDeclaration* fDecl = fRefExp->getAssociatedFunctionDeclaration();
					if (!fDecl) {
						std::cout << "NMSelector: Declaration of a SgFunctionRefExp was null." << std::endl;
					}
					select(fDecl);
					return;
				}	//
				select(n);
			}
		}
	}
	else if (postOrderMatch)
	{

	}
}
void NameMatchingASTTraversal::postOrderVisit(SgNode* n) {
	if (preOrderMatch){
		if (n == nodeTracker.top())
			nodeTracker.pop();
	}
	else if (postOrderMatch){

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

}}}} // Cose the namespaces
