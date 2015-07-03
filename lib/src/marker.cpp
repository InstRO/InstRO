#include "instro/rose/utility/marker.h"

using namespace InstRO;

/* Static method
 * \brief Retrieve the Attribute Container. Returns NULL if no container is attached to node.
 *
 */
ASTAttributeContainer* ASTAttributeContainer::hasASTAttributeContainer(SgNode* node){
    if(node == NULL){
        std::cerr << "NODE WAS NULL" << std::endl;
        //exit(-1); // If node is null, we will have a seg-fault in code.
        throw InstroException("In ASTAttributeContainer::hasASTAttributeContainer: node was NULL");
    }
    //	std::cout << "getASTAttributeContainer entry" << std::endl;
    AstAttribute* attrib = node->getAttribute(ROSE_AST_CONTAINER_ATTRIB_NAME);
    if(attrib != NULL){
        //		std::cout << "attrib not equal NULL" << std::endl;
        ASTAttributeContainer* container = dynamic_cast<ASTAttributeContainer*>(attrib);
        if(container != NULL){
            //			std::cout << "container not NULL" << std::endl;
            return container;
        }
    }
    return NULL;
}

/* Static method
 * \brief If no container it creates a container object and returns a pointer to the newly created
 * instance. If an object already existed, it simply returns a pointer to the object.
 *
 */
ASTAttributeContainer* ASTAttributeContainer::getASTAttributeContainer(SgNode* n){
    if(n == NULL){
        std::cerr << "FAILURE in attachASTAttributeContainer: Node passed was NULL" << std::endl;
        return NULL;
    }
    AstAttribute* attrib = n->getAttribute(ROSE_AST_CONTAINER_ATTRIB_NAME);
    ASTAttributeContainer* container = dynamic_cast<ASTAttributeContainer*>(attrib);
    if(container == NULL){
        container = new ASTAttributeContainer();
        n->setAttribute(ROSE_AST_CONTAINER_ATTRIB_NAME, container);
    }
    return container;
}

/**
 * \brief creates an empty AttributeContainer
 *
 */
ASTAttributeContainer::ASTAttributeContainer(){
    this->selectorMarkerMap = std::map<size_t, SharedAttributePointerType>();
}

ASTAttributeContainer::~ASTAttributeContainer(){
#ifdef DEBUG
	std::cout << "Container destructor called." << std::endl;
#endif
}

/*
 *\brief Inserts the attrib into the container's map
 *
 */
void ASTAttributeContainer::setSelAttrib(size_t selId, SharedAttributePointerType attrib){
    /*	if(this->selectorAttributeMap.count(selId) > 0){
  this->selectorAttributeMap.erase(selId);
 }
*/	// No element to this key exists, so its simply inserted
    //	std::cout << "Inserting pair with key: " << selId << "and attrib with ref count: " << attrib.use_count() << std::endl;
    this->selectorMarkerMap.insert(std::pair<size_t, SharedAttributePointerType>(selId, attrib));
    //	std::cout << "In attribute set the count for Id " << selId << " with attrib is: " << attrib.use_count() << std::endl;
}

/*
 * \brief Removes the attribute corresponding to the selector's Id from the container's map.
 *
 */
void ASTAttributeContainer::remSelAttrib(size_t selId){
    this->selectorMarkerMap.erase(selId);
}

/*
 * \brief Gets the selection attribute corresponding to selId
 */
ASTAttributeContainer::SharedAttributePointerType ASTAttributeContainer::getSelAttrib(size_t selId){
    return (this->selectorMarkerMap.find(selId))->second;
}

/*
 * \brief Checks if a selection attribute corresponding to selId exists
 */
bool ASTAttributeContainer::hasSelAttrib(size_t selId){
    //	std::cout << "Request for Id: " << selId << std::endl;
    //	std::cout << "Number of elements in the container: " << this->selectorAttributeMap.size() << std::endl;
    return (this->selectorMarkerMap.count(selId) > 0)? true : false;
}


/*
 *
 */
void SelectionASTMarker::select(Selector * sel){
    selectingSelector = sel;
    /// FIXME 2013-10-07 JP: Here comes in a circular dependency, as we use a method of an incomplete type!
    // Any Ideas? :)
//    this->markersLedToSelection = this->selectingSelector->getSelectionDecisionMarkers();
}

/*
 *
 */
Selector* SelectionASTMarker::getSelectingSelector(){
    return this->selectingSelector;
}

std::vector<ASTAttributeContainer::SharedAttributePointerType > SelectionASTMarker::getSelectionBase(){
    return this->selectionBase();
}

std::vector<ASTAttributeContainer::SharedAttributePointerType > SelectionASTMarker::selectionBase(){
    return this->markersLedToSelection;
}

/* ## NameMatchingSelectorASTAttribute ## */

NameMatchingSelectionASTMarker::NameMatchingSelectionASTMarker(Selector* sel){
    this->select(sel);
}

void NameMatchingSelectionASTMarker::addMatchToList(std::string str){
    matches.push_back(str);
}

std::list<std::string> NameMatchingSelectionASTMarker::getMatches(){
    return matches;
}

BWSelectionASTMarker::BWSelectionASTMarker(Selector* selectingSelector){
    this->selectingSelector = selectingSelector;
}

Selector* BWSelectionASTMarker::getSelectingSelector(){
    return this->selectingSelector;
}

/*
 * \brief Adds all elements of idsToAddToList at the end of the matchingIdsList
 * \todo 2013-10-10 JP: Do not store integers but all matched strings
 */
void BWSelectionASTMarker::addIdsToList(std::vector<int> idsToAddToList){
    std::vector<int>::iterator it;
    for(it = matchingIdsList.begin(); it != matchingIdsList.end(); it++){
        matchingIdsList.push_back(*it);
    }
}

/*
 * returns a std::vector of all Ids which match
 */
std::vector<int> BWSelectionASTMarker::getListOfMatchingIds(){
    return matchingIdsList;
}

/*
 * Set Selector definitions.
 */
/*
void CompoundSelectionASTMarker::select(Selector* sel){
    this->selectingSelector = sel;
}
*/
Selector* CompoundSelectionASTMarker::getSelectingSelector(){
    return this->selectingSelector;
}

/* --- Geimer Selector --- */
void GeimerSelectionASTMarker::setSpecNodes(std::vector<SgNode*> nodes){
    this->specNodes = nodes;
}

void GeimerSelectionASTMarker::setIncludeFiles(std::vector<std::string> includeFiles){
    this->includeFiles = includeFiles;
}

std::vector<SgNode*> GeimerSelectionASTMarker::getSpecNodes(){
    return this->specNodes;
}

std::vector<std::string> GeimerSelectionASTMarker::getIncludeFilenames(){
    return this->includeFiles;
}
