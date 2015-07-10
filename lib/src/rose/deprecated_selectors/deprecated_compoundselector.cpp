#include "instro/selectors/compoundselector.h"
#define VERBOSE 0
/* CompoundSelector */
CompoundSelector::CompoundSelector() {}

CompoundSelector::CompoundSelector(std::vector<Selector*> selectors, std::vector<bool> inversions)
		: subSelectors(selectors), invSelectors(inversions) {
	if (selectors.size() != inversions.size()) {
		std::cerr << "You have to specify as many booleans as you add selectors to set selector with id: " << this->getId()
							<< std::endl;
		// exit(-1);
		throw InstroException(
				"CompoundSelector: Trying to create instance with unbalanced number of selectors and inversion specifiers.");
	}
}

CompoundSelector::CompoundSelector(std::vector<Selector*> selectors) : subSelectors(selectors) {
	this->invSelectors = std::vector<bool>(selectors.size(), false);
}

void CompoundSelector::traverseAST(SgNode* start) {
	// starts the processing of all nodes
	startNodeProcessing(start);
}

void CompoundSelector::applyOperation(SgNode* n) {
	// apply the actual operation
	// XXX consider the to do about having an inversion list.
}

void CompoundSelector::startNodeProcessing(SgNode* startNode) {
	//    std::cout << "Start node processing in CompoundSelector with id: " << this->getId() << std::endl;
	// generate node list
	// call processNode for each node in list
}

void CompoundSelector::processNode(SgNode* n) {
	// Do we have to prepare anything for applying the operation...?
	// process it
	applyOperation(n);
}

SelectionASTMarker* CompoundSelector::createSelectionMarker() { return new CompoundSelectionASTMarker(); }

void CompoundSelector::getSubSelectors(std::list<Selector*>& subSelectorList) {
	for (std::vector<Selector*>::iterator it = subSelectors.begin(); it != subSelectors.end(); it++) {
		subSelectorList.push_back(*it);
	}
}

/**
 * Set Selector
 * This constructor takes two std::vectors, the first with the actual selector instances, the second with as many
 * bools as selectors, to indicate if selector at index i should inverted. A "true" in the inversion vector means,
 * that the appearance of a selector marker should be interpreted inverted. (Meaning if there is a marker it will be
 * seen as if it wasn't attached and vice versa.
 */
SetSelector::SetSelector(std::vector<Selector*> sels, std::vector<bool> invList) : CompoundSelector(sels, invList) {}

/**
 * In an n-ary intersection we return false as soon as one literal is negative.
 * Therefore all added selectors are processed by searching for those instances in the ASTAttributeContainer map
 * attached to the node.
 */
bool SetSelector::evalNaryIntersection(ASTAttributeContainer* selCon) {
	std::vector<Selector*>::iterator i;
	int j = 0;	// to go through the inversion vector
	for (i = this->subSelectors.begin(); i != this->subSelectors.end(); i++) {
		if (!selCon->hasSelAttrib((*i)->getId())) {
			if (this->invSelectors.at(j))
				continue;	// XXX Is this semantiv correct?
			return false;
		}
		j++;
	}
	return true;	// if we got here we have to return true
}

/**
 * In an n-ary union we return true as soon as one positive literal is found.
 * Therefore all added selectors are processed by searching for those instances in the ASTAttributeContainer map
 * attached to the node.
 */
bool SetSelector::evalNaryUnion(ASTAttributeContainer* selCon) {
	std::vector<Selector*>::iterator i;
	int j = 0;	// to go through the inversion vector
	for (i = this->subSelectors.begin(); i != this->subSelectors.end(); i++) {
		if (selCon->hasSelAttrib((*i)->getId())) {
			if (this->invSelectors.at(j))
				continue;
			return true;
		}
		j++;
	}
	return false;	// if we got here, we have to return false
}

/* BooleanCompoundSelector */
BooleanCompoundSelector::BooleanCompoundSelector(Selector* sel1, bool invSel1, Selector* sel2, bool invSel2,
																								 OP_Enum operation)
		: CompoundSelector(std::vector<Selector*>(), std::vector<bool>()) {
	subSelectors.push_back(sel1);
	invSelectors.push_back(invSel1);
	subSelectors.push_back(sel2);
	invSelectors.push_back(invSel2);

	this->operation = operation;
}

void BooleanCompoundSelector::startNodeProcessing(SgNode* startNode) {
	// XXX RN: preferWhitelist==true allows to select nodes that are on neither WL nor BL !
	// JP: Was this issue fixed in some way? I thought that it was the intent when I discussed the semantics with CI a
	// long time ago...
	std::cout << "Start node processing in BooleanCompoundSelector with id: " << this->getId() << std::endl;
	// generate list of nodes which have to be evaluated
	MasterFunctionalSelector<MarkerAttributeFunctional> mfs_a = MasterFunctionalSelector<MarkerAttributeFunctional>(
			startNode, MarkerAttributeFunctional(this->subSelectors.at(0)->getId()));
	MasterFunctionalSelector<MarkerAttributeFunctional> mfs_b = MasterFunctionalSelector<MarkerAttributeFunctional>(
			startNode, MarkerAttributeFunctional(this->subSelectors.at(1)->getId()));
	Rose_STL_Container<SgNode*> listA = mfs_a.retrieveNodesWithAttribute(startNode);
	Rose_STL_Container<SgNode*> listB = mfs_b.retrieveNodesWithAttribute(startNode);

	// Evaluate on two lists (first copy all elements into one list
	Rose_STL_Container<SgNode*> evalList;

	// --- JPL: This ugly copying is done to overcome a NULL pointer problem.
	Rose_STL_Container<SgNode*>::iterator copyIter;
	for (copyIter = listA.begin(); copyIter != listA.end(); copyIter++) {
		evalList.push_back((*copyIter));
	}
	for (copyIter = listB.begin(); copyIter != listB.end(); copyIter++) {
		evalList.push_back((*copyIter));
	}
	// ---

	//	// XXX RN: for now evaluate all function definitions
	//	Rose_STL_Container<SgNode*> evalList = NodeQuery::querySubTree(startNode, V_SgFunctionDefinition);

	// iterate over all elements
	Rose_STL_Container<SgNode*>::iterator iter;
	int i = 1;
	for (iter = evalList.begin(); iter != evalList.end(); iter++) {
		if ((*iter) == NULL) {
			std::cerr << "ERROR: The node in iterating over the nodes was null" << std::endl;
		}
		i++;
		processNode(*iter);	// process each node
	}
}

void BooleanCompoundSelector::processNode(SgNode* n) {
	ASTAttributeContainer* container = ASTAttributeContainer::hasASTAttributeContainer(n);
	if (container == NULL) {
		std::cerr << "container is null in BooleanCompoundSelector::processNode. This should NOT have happened."
							<< std::endl;
		// exit(-1);
		throw InstroException("Container in BooleanCompoundSelector is NULL. This should not have happened.");
	}
	// Here container can't be NULL
	bool state = false;
	// switch in operation to distinguish what has to be evaluated
	switch (operation) {
		// binary intersection
		case BIN_INTERSECTION:
			state = evalLogicalAnd(container);
			if (state)
				select(n);
			break;
		// binary union
		case BIN_UNION:
			state = evalLogicalOr(container);
			if (state)
				select(n);
			break;
		// binary implication
		case BIN_IMPLICATION:
			state = evalLogicalImplication(container);
			if (state)
				select(n);
			break;
	}
}

void BooleanCompoundSelector::traverseAST(SgNode* start) {
	// XXX RN: no need to delegate sub-selectors any longer
	//	std::vector<Selector*>::iterator selIter;
	//		for(selIter = subSelectors.begin(); selIter != subSelectors.end(); selIter++){
	//		// Let all for this set selector necessary selectors run
	//		std::cout << "Running Selector with ID: " << (*selIter)->getId() << " @ project: " << &start << std::endl;
	//		(*selIter)->traverseAST(start);
	//	}
	//	std::cout << "Finished selection for subset selectors." << std::endl;

	// Start processing with this selector
	startNodeProcessing(start);
}

SelectionASTMarker* BooleanCompoundSelector::createSelectionMarker() {
#if VERBOSE > 2
	std::cout << "BooleanCompoundSelector marked node" << std::endl;
#endif
	return new CompoundSelectionASTMarker();
}

/*
 * Evaluation of binary logical operations
 */
bool BooleanCompoundSelector::evalLogicalAnd(ASTAttributeContainer* selCon) {
	//	std::cout << "The Id used to retrieve Attribute in evalBinIntersect: " << this->selectorsToUse.at(0)->getId() <<
	// std::endl;
	// Get first Argument
	bool a = selCon->hasSelAttrib(this->subSelectors.at(0)->getId());
	//	std::cout << (a? "a: true" : "a: false") << std::endl;
	if (invSelectors.at(0)) {
		a = !a;
	}
	//	std::cout << (a? "a: true" : "a: false") << std::endl;
	// Get second Argument
	bool b = selCon->hasSelAttrib(this->subSelectors.at(1)->getId());
	//	std::cout << (b? "b: true" : "b: false") << std::endl;
	if (invSelectors.at(1)) {
		b = !b;
	}
	//	std::cout << (b? "b: true" : "b: false") << std::endl;
	return (a && b);
}

bool BooleanCompoundSelector::evalLogicalOr(ASTAttributeContainer* selCon) {
	// Get first Argument
	bool a = selCon->hasSelAttrib(this->subSelectors.at(0)->getId());
	if (invSelectors.at(0)) {
		a = !a;
	}
	// Get second Argument
	bool b = selCon->hasSelAttrib(this->subSelectors.at(1)->getId());
	if (invSelectors.at(1)) {
		b = !b;
	}
	return (a || b);
}

bool BooleanCompoundSelector::evalLogicalImplication(ASTAttributeContainer* selCon) {
	// Get first Argument
	bool a = selCon->hasSelAttrib(this->subSelectors.at(0)->getId());
	if (invSelectors.at(0)) {
		a = !a;
	}
	// Get second Argument
	bool b = selCon->hasSelAttrib(this->subSelectors.at(1)->getId());
	if (invSelectors.at(1)) {
		b = !b;
	}
	return ((!a) || b);
}

bool BooleanCompoundSelector::evalLogicalXor(ASTAttributeContainer* selCon) {
	// Get first Argument
	bool a = selCon->hasSelAttrib(this->subSelectors.at(0)->getId());
	if (invSelectors.at(0)) {
		a = !a;
	}
	// Get second Argument
	bool b = selCon->hasSelAttrib(this->subSelectors.at(1)->getId());
	if (invSelectors.at(1)) {
		b = !b;
	}
	return ((!!a) ^ (!!b));	// xor TODO: Check for correctnes!
}
