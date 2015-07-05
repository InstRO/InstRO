#ifndef INSTRO_FUNCTIONALS_H__
#define INSTRO_FUNCTIONALS_H__

#include <rose.h>	// For the SgNode def.

#include "marker.h"

namespace InstRO {

/**
 * \brief Traverses the AST and returns list of all nodes holding an ASTAttributeContainer
 *
 * This Functional is used to retrieve all nodes in the ast which hold an SelectionASTAttributeContainer and
 * return them in a list.
 * This is useful basic functionality as well as an example for users or developers, to see how a functional,
 * used in the GenericAttributeSelector should look like.
 *
 * \todo 2013-10-28 JP: Separate declaration and definition?
 *
 * \ingroup Util
 * \author Jan-Patrick Lehr
 */
class ContainerAttributeFunctional : public std::unary_function<SgNode*, Rose_STL_Container<SgNode*> > {
 public:
	ContainerAttributeFunctional(){};

	result_type operator()(SgNode* n) {
		result_type returnList;

		if (ASTAttributeContainer::hasASTAttributeContainer(n) != NULL) {
			returnList.push_back(n);
			//            std::cout << "Found container and marker with id:" << selectorId << " !"<< std::endl;
		}

		return returnList;
	}

 private:
	size_t selectorId;
};

/**
 * \brief Traverses the AST and returns a list of nodes holding an ASTAttribute with name attribName
 *
 * Generic AstAttribute retrieval functional. Expects a string with the ast attribute name.
 * \ingroup Util
 * \author Jan-Patrick Lehr
 */
class AstAttributeFunctional : public std::unary_function<SgNode*, Rose_STL_Container<SgNode*> > {
 public:
	AstAttributeFunctional(std::string attribName = ROSE_AST_CONTAINER_ATTRIB_NAME) { this->attribName = attribName; };

	result_type operator()(SgNode* n) {
		result_type returnList;

		AstAttribute* attrib = n->getAttribute(attribName);
		if (attrib != NULL) {
			returnList.push_back(n);
			//          std::cout << "Found attribute for string:" << attribName << " !"<< std::endl;
		}

		return returnList;
	}

 private:
	std::string attribName;
};

/**
 * \brief Traverses the AST and generates a list of nodes, if they are marked by a Selector with selectorID
 *
 * This Functional is used to retrieve all nodes in the ast which hold a SelectionASTMarker to selector with
 * a special id.
 * \todo Separate declaration and definition?
 * \ingroup Util
 * \author Jan-Patrick Lehr
 */
class MarkerAttributeFunctional : public std::unary_function<SgNode*, Rose_STL_Container<SgNode*> > {
 public:
	MarkerAttributeFunctional(){};
	// For container retrieval this constructor is not neccessary! Maybe removed
	MarkerAttributeFunctional(size_t selectorId) { this->selectorId = selectorId; };

	result_type operator()(SgNode* n) {
		result_type returnList;

		if (ASTAttributeContainer::hasASTAttributeContainer(n) != NULL) {
			if (ASTAttributeContainer::hasASTAttributeContainer(n)->hasSelAttrib((selectorId))) {
				returnList.push_back(n);
				//                std::cout << "Found container and marker with id:" << selectorId << " @ " <<
				//                SageInterface::get_name(n) << " "<< std::endl;
			}
		}

		return returnList;
	}

 private:
	size_t selectorId;
};

/**
 * \brief Is passed one of the AST functionals. Starts traversal and generates a list of nodes where the used functional
 *evaluates to true.
 *
 *
 * The MasterFunctionalSelector abstracts the need to start the traversal of the AST.
 * Simply one of the AST attribute functionals is provided and the MasterFunctionalSelector generates a list of nodes
 *which have to be processed.
 * At the moment there is the possibility to pass the ContainerAttributeFunctional (or one of the others),
 * to retrieve all AST-nodes which have a SelectionASTAttributeContainer object attached. This list can be
 * generated and retrieved using the retrieveNodesWithAttribute() method.
 *
 * \ingroup Util
 * \author Jan-Patrick Lehr
 */
template <class T>
class MasterFunctionalSelector {
 public:
	MasterFunctionalSelector(SgNode* topNode);	// Creates new instance of T (i.e. for the ContainerAttributeFunctional)
	MasterFunctionalSelector(SgNode* topNode, T obj);
	/** Searches for all nodes which have a SelectionASTAttributeContainer attached and does nothing with them. */
	Rose_STL_Container<SgNode*> retrieveNodesWithAttribute();

	Rose_STL_Container<SgNode*> retrieveNodesWithAttribute(SgNode* startNode);

 private:
	SgNode* topNode;
	T rObj;
};
}

using namespace InstRO;
// ------------------
template <class T>
MasterFunctionalSelector<T>::MasterFunctionalSelector(SgNode* topNode) {
	rObj = T();
	this->topNode = topNode;
}

template <class T>
MasterFunctionalSelector<T>::MasterFunctionalSelector(SgNode* topNode, T obj) {
	rObj = obj;
	this->topNode = topNode;
}

template <class T>
Rose_STL_Container<SgNode*> MasterFunctionalSelector<T>::retrieveNodesWithAttribute() {
	Rose_STL_Container<SgNode*> nodeList = NodeQuery::querySubTree(topNode, rObj);

	//    std::cout << "nodeList" << &nodeList << std::endl;
	return nodeList;
}

template <class T>
Rose_STL_Container<SgNode*> MasterFunctionalSelector<T>::retrieveNodesWithAttribute(SgNode* start) {
	Rose_STL_Container<SgNode*> nodeList = NodeQuery::querySubTree(start, rObj);

	//  std::cout << "nodeList" << &nodeList << std::endl;
	return nodeList;
}

#endif
