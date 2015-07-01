#include "rose.h"

#include <memory> // We need shared pointers
#include <list> // We use List in the GrammarInterface
#include "instro/core/ConstructSet.h"

namespace InstRO{ namespace Rose {	namespace Tooling	{	namespace NamedConstructAccess {

class NameMatchingASTTraversal : public AstPrePostProcessing{

void preOrderVisit(SgNode* n) {
	if (this->matchingObject == NULL || this->listToMatchAgainst == NULL) {
		std::cerr << "Matching object and/or listToMatchAgainst are NULL. Please use init(...) before invoking the traversal." << std::endl;
		throw InstroException("NameMatchingSelector was not initialized before starting traversal.");
	}
	
	// generate a string from the rose AST node ...
	stdd::string generatedName = toString(n);
	if (generatedName.compare("INSTRUMENTOR_NOT_SUPPORTED_ATM") != 0) {
			// Check the generated name against the stored list
				/* 2013-10-15 JP: This was based on a previous implementation in the matcher.
				lastMatchIds = this->matchingObject->match(generatedName, *listToMatchAgainst);
				if (lastMatchIds.size() > 0) {
				*/

				if (this->matchingObject->bMatch(generatedName, *listToMatchAgainst)) {

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
};

}}}} // Cose the namespaces
