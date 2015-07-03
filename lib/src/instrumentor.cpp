#include <instro/rose/instrumentor.h>

using namespace InstRO;

void Selector::select(SgNode* node) {
	ASTAttributeContainer* container = ASTAttributeContainer::getASTAttributeContainer(node);
	ASTAttributeContainer::SharedAttributePointerType selMarker; // not sure if sensible to allocate a NULL-Shared pointer

	if (!container->hasSelAttrib(this->getId())) {
		//SelectionASTMarker* sm = createSelectionMarker();
		//selMarker( sm ); // Does this work?
		selMarker = ASTAttributeContainer::SharedAttributePointerType(createSelectionMarker());
		selectedNodes++;
	} else {
		selMarker = container->getSelAttrib(this->getId());
	}
	selMarker->select(this);
	container->setSelAttrib(this->getId(), selMarker);
}

bool Selector::isSelected(SgNode* node) {
	ASTAttributeContainer* container = ASTAttributeContainer::getASTAttributeContainer(node);
	return container->hasSelAttrib(this->getId());
}

void Selector::deselect(SgNode* node) {
	ASTAttributeContainer* container = ASTAttributeContainer::getASTAttributeContainer(node);
	container->remSelAttrib(this->getId());
}

SelectionASTMarker * Selector::getSelectionMarker(SgNode* node) {
	ASTAttributeContainer* container = ASTAttributeContainer::getASTAttributeContainer(node);
	if (!container->hasSelAttrib(this->getId())) {
		return NULL;
	}
	ASTAttributeContainer::SharedAttributePointerType sap = container->getSelAttrib(this->getId());
	return sap.get(); // here we return a SelectionASTMarker* see comment at declaration!!
}

SelectionASTMarker * Selector::createSelectionMarker() {
	return new SelectionASTMarker();
}

size_t Selector::getId() {
	return reinterpret_cast<size_t>(this);
}

int Selector::getNumberOfMarkedNodes() {
	return selectedNodes;
}
std::vector<ASTAttributeContainer::SharedAttributePointerType> Selector::getSelectionDecisionMarkers() {
	std::vector<ASTAttributeContainer::SharedAttributePointerType> decisionBase;

	std::list<Selector*> localSelectors;
	getSubSelectors(localSelectors); // XXX

	std::list<Selector*>::iterator iter;
	std::vector<ASTAttributeContainer::SharedAttributePointerType> decisionbaselocal;
	for(iter = localSelectors.begin(); iter != localSelectors.end(); iter++){
		decisionbaselocal = (*iter)->getSelectionDecisionMarkers();
		if(decisionbaselocal.size() > 0){
			decisionBase.insert(decisionBase.end(),decisionbaselocal.begin(), decisionbaselocal.end());
		}
	}

	return std::vector<ASTAttributeContainer::SharedAttributePointerType>();
}

