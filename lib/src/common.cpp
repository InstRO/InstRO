#include "instro/common.h"

#define VERBOSE 0

using namespace InstRO;

/** BasicInstrumentor
 *  Initialize Member Variables to sane values */
#include <iostream>

BasicInstrumentor::BasicInstrumentor() : project(0) {}

void BasicInstrumentor::select() {
	if (project == 0) {
		return;
	}
	for (std::list<Selector*>::iterator sel = selectorList.begin(); sel != selectorList.end(); sel++) {
		(*sel)->traverseAST(project);
	}

	return;
}

void BasicInstrumentor::instrument() {
	if (project == 0) {
		return;
	}

	std::list<AdapterBuilder*>::iterator builderIter;
	for (builderIter = adapterBuilderList.begin(); builderIter != adapterBuilderList.end(); builderIter++) {
		(*builderIter)->validateInternalState();
		(*builderIter)->startModificationPass();
	}

	return;
}

void BasicInstrumentor::setProject(SgProject* project) { this->project = project; }

SgProject* BasicInstrumentor::getProject() { return this->project; }

void BasicInstrumentor::addSelector(Selector* sel) {
	// Check if the selector is already registered
	if (hasSelector(sel)) {
		return;
	}

	// register sub-selectors
	std::list<Selector*> subSelectors;
	sel->getSubSelectors(subSelectors);

	for (std::list<Selector*>::iterator i = subSelectors.begin(); i != subSelectors.end(); i++) {
		// RN: do not register sub-selectors twice
		if (hasSelector(*i)) {
			continue;
		}
		addSelector(*i);
	}

#if VERBOSE
	std::cout << "InstRO: added Sel " << sel->getId() << std::endl;
#endif
	// Since we got here, the selector has not been registered yet
	selectorList.push_back(sel);
}

void BasicInstrumentor::removeSelector(Selector* sel) {
	for (std::list<Selector*>::iterator i = selectorList.begin(); i != selectorList.end(); i++) {
		if (*i == sel) {
			selectorList.erase(i);
			return;
		}
	}
}

bool BasicInstrumentor::hasSelector(Selector* sel) {
	for (std::list<Selector*>::iterator i = selectorList.begin(); i != selectorList.end(); i++) {
		if (*i == sel) {
			return true;
		}
	}
	return false;
}

void BasicInstrumentor::addAdapterBuilder(AdapterBuilder* adapterBuilder) {
	assert(project != NULL);	// TODO 2013-10-22 JP: Use exceptions here?
	this->adapterBuilderList.push_back(adapterBuilder);
	adapterBuilder->setProject(this->project);
}

void BasicInstrumentor::removeAdapterBuilder(AdapterBuilder* adapterBuilder) {
	for (std::list<AdapterBuilder*>::iterator iter = adapterBuilderList.begin(); iter != adapterBuilderList.end();
			 ++iter) {
		if ((*iter) == adapterBuilder) {
			adapterBuilderList.remove(*iter);
		}
	}
}

void BasicInstrumentor::addTransformer(Transformer* transformer) {
	assert(project != NULL);	// TODO 2013-10-22 JP: Use exceptions here?
	this->transformerList.push_back(transformer);
	transformer->setProject(this->project);
}

void BasicInstrumentor::removeTransformer(Transformer* transformer) {
	for (std::list<Transformer*>::iterator iter = transformerList.begin(); iter != transformerList.end(); ++iter) {
		if ((*iter) == transformer) {
			transformerList.remove(*iter);
		}
	}
}

/* AttributeSelector */
AttributeSelector::AttributeSelector(std::string attribName) {
	int i;
	this->attribName = attribName;
}

void AttributeSelector::traverseAST(SgNode* start) {
	// starts the processing of nodes.
	startNodeProcessing(start);
}

void AttributeSelector::startNodeProcessing(SgNode* startNode) {
	// have this in a private method?
	MasterFunctionalSelector<AstAttributeFunctional> mfs =
			MasterFunctionalSelector<AstAttributeFunctional>(startNode, AstAttributeFunctional(this->attribName));
	// generate node-list
	this->nodesList = mfs.retrieveNodesWithAttribute(startNode);

	// foreach [node] processNode(node). This could be public...
	Rose_STL_Container<SgNode*>::iterator iter;
	for (iter = nodesList.begin(); iter != nodesList.end(); iter++) {
		processNode((*iter));
	}
}

void AttributeSelector::processNode(SgNode* n) {
	// what exactly is done here?
	select(n);
}

/* PrePostSelector */
void PrePostOrderSelector::traverseAST(SgNode* start) {
	this->selectionBegin(SageInterface::getEnclosingNode<SgProject>(start, true));
	traverse(start);
	this->selectionEnd(SageInterface::getEnclosingNode<SgProject>(start, true));
}

void PrePostOrderSelector::selectionBegin(SgProject* project) {}

void PrePostOrderSelector::selectionEnd(SgProject* project) {}

void PrePostOrderSelector::preOrderVisit(SgNode* node) {}

void PrePostOrderSelector::postOrderVisit(SgNode* node) {}

/* OutOfOrderSelector */
void OutOfOrderSelector::traverseAST(SgNode* start) {
	this->selectionBegin(SageInterface::getEnclosingNode<SgProject>(start, true));
	traverse(start, preorder);
	this->selectionEnd(SageInterface::getEnclosingNode<SgProject>(start, true));
}

void OutOfOrderSelector::selectionBegin(SgProject* project) {}

void OutOfOrderSelector::selectionEnd(SgProject* project) {}

bool ASTReWriterImpl::isNodeSelected(SgNode* node) {
	if (ASTAttributeContainer::hasASTAttributeContainer(node)) {
		ASTAttributeContainer* container = ASTAttributeContainer::getASTAttributeContainer(node);
#ifdef DEBUG
		std::cout << "Should node be instrumented: " << container->hasSelAttrib(decidingSelector->getId()) << std::endl;
		std::cout << "Listening to: " << decidingSelector << std::endl;
#endif
		return container->hasSelAttrib(decidingSelector->getId());
	}
	return false;
}

ASTReWriterImpl::ASTReWriterImpl(Selector* selectorToRespect) : decidingSelector(selectorToRespect), project(NULL) {}

void ASTReWriterImpl::validateInternalState() {
	if (decidingSelector == NULL || project == NULL)
		throw InstroException("Validate internal state in ASTReWriterImpl encountered an error.");
}

void ASTReWriterImpl::setProject(SgProject* proj) { this->project = proj; }

std::vector<SgNode*> ASTReWriterImpl::generateSelectedNodeSet() {
	assert(project != NULL);

	MasterFunctionalSelector<MarkerAttributeFunctional> cas = MasterFunctionalSelector<MarkerAttributeFunctional>(
			project, MarkerAttributeFunctional(decidingSelector->getId()));

	return cas.retrieveNodesWithAttribute();
}

const Selector* ASTReWriterImpl::getSelector() { return this->decidingSelector; }

void AdapterBuilder::startModificationPass() {
	std::vector<SgNode*> nodes = generateSelectedNodeSet();

	std::vector<SgNode*>::iterator nodesIter;
	for (nodesIter = nodes.begin(); nodesIter != nodes.end(); nodesIter++) {
		buildForNode((*nodesIter));
	}

	modificationEnd();
}
