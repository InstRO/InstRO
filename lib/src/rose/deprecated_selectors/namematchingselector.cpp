#include <instro/selectors/namematchingselector.h>

using namespace InstRO;

NameMatchingSelector::NameMatchingSelector() : verbose(false), isInitialized(false) {}

NameMatchingSelector::NameMatchingSelector(bool verbose) : verbose(verbose), isInitialized(false) {}

void NameMatchingSelector::init(IN_enum nodetypeToMark, Matcher* matcherToUse,
																std::list<std::string>* listToMatchAgainst) {
	this->nodetypeToMark = nodetypeToMark;
	this->matchingObject = matcherToUse;
	this->listToMatchAgainst = listToMatchAgainst;
	if (verbose) {
		std::cout << "=== Verbose Output from " << this << " NameMatchingSelector ===\n";
		std::cout << "The NameMatchingSelector was initialized with:\n";
		std::cout << "nodetypeToMark: " << nodetypeToMark << "\n";
		std::cout << "matchingObject: " << matchingObject << "\n";
		std::cout << "listToMatchAgainst: " << listToMatchAgainst << " : [\n";
		for (std::list<std::string>::iterator i = listToMatchAgainst->begin(); i != listToMatchAgainst->end(); i++) {
			std::cout << "+ " << *i << "\n";
		}
		std::cout << "]" << std::endl;
	}
	this->isInitialized = true;
}

void NameMatchingSelector::preOrderVisit(SgNode* n) {
	if (isInitialized) {
		if (this->matchingObject == NULL || this->listToMatchAgainst == NULL) {
			std::cerr
					<< "Matching object and/or listToMatchAgainst are NULL. Please use init(...) before invoking the traversal."
					<< std::endl;
			throw InstroException("NameMatchingSelector was not initialized before starting traversal.");
		}
		// implements what to do at a node
		std::string generatedName = toString(n);
		if (generatedName.compare("INSTRUMENTOR_NOT_SUPPORTED_ATM") != 0) {
			// Check the generated name against the stored list
			/* 2013-10-15 JP: This was based on a previous implementation in the matcher.
			lastMatchIds = this->matchingObject->match(generatedName, *listToMatchAgainst);
			if (lastMatchIds.size() > 0) {
			*/

			if (this->matchingObject->bMatch(generatedName, *listToMatchAgainst)) {
				if (verbose) {
					std::cout << "Matcher tried to match: " << generatedName << " and returned " << true
										<< ". Leading to node selection." << std::endl;
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
	} else {
		// XXX 2013-10-08 JP: Throw an exception here as well, or something?
		std::cerr << "Object (NameMatchingSelector @ " << this << " ) has not been initialized." << std::endl;
	}
}

std::vector<std::string> NameMatchingSelector::retrieveMatches(SgNode* n) {
	// returns all matches to this node
	ASTAttributeContainer* container = ASTAttributeContainer::getASTAttributeContainer(n);
	if (!container->hasSelAttrib(this->getId())) {
		// XXX 2013-10-08 JP: Maybe this output is unnecessary (depends on which node type was marked?)
		std::cerr << "Error: NameMatchingSelector::retrieveMatches called on node where no NameMatchingSelector with Id: "
							<< this->getId() << " attribute was stored" << std::endl;
	} else {
		ASTAttributeContainer::SharedAttributePointerType ap = container->getSelAttrib(this->getId());
		SelectionASTMarker* p = ap.get();
		NameMatchingSelectionASTMarker* nmAp = dynamic_cast<NameMatchingSelectionASTMarker*>(p);
		std::list<std::string> matches = nmAp->getMatches();
		std::vector<std::string> retVec;
		std::list<std::string>::iterator i;
		for (i = matches.begin(); i != matches.end(); i++) {
			retVec.push_back((*i));
		}
		return retVec;
	}
	return std::vector<std::string>();
}

std::string NameMatchingSelector::toString(SgNode* n) {
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

SelectionASTMarker* NameMatchingSelector::createSelectionMarker() {
	NameMatchingSelectionASTMarker* marker = new NameMatchingSelectionASTMarker(this);

	// XXX 2013-10-08 JP: This can be done more beautiful?
	WildcardedStringMatcher* wsm = dynamic_cast<WildcardedStringMatcher*>(this->matchingObject);
	if (wsm) {
		std::vector<std::string> tempVec;
		tempVec = wsm->getMatchingStringList();
		std::vector<std::string>::iterator i;
		for (i = tempVec.begin(); i != tempVec.end(); i++) {
			marker->addMatchToList((*i));
		}
	}
	return marker;
}
