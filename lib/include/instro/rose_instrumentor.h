#ifndef INSTRUMENTOR_H__
#define INSTRUMENTOR_H__

#include <rose.h>
#include "utility/marker.h"

/**
 * \defgroup Selector Selectors
 * \defgroup ASTReWriter Astrewriter
 * \defgroup Adapterbuilder Adapterbuilders
 * \defgroup Instrumentor Instrumentors
 * \defgroup Transformer Transformers
 * \defgroup Markers AST Selectionmarkers
 * \defgroup Util Utilities
 * \defgroup InstROExceptions exceptions
 *
 * \todo 2013-10-26 JP: How in doxy to define "a group inherits from another"?
 */

namespace InstRO {


/**
 * 2013-10-22 JP: I moved everything derived from one of these three interface definitions to common.h
 *
 * We should decide whether we want to have Selector, ASTModifier and Instrumentor to be interfaces. Which have an
 * abstract derived class in common.h to implement common functionality.
 *
 * This would be more like the whole Instrumentor-thing as an idea and InstRO as concrete implementation.
 *
 * 2013-10-26 JP: If I am right, then the goal is to have the Instrumentor as an interface, but we decided to see
 * the .h file as the interface declaration and the .cpp as the interface definition.
 * Therefore we may get rid of the java-style ASTReWriterImpl class.
 *
 */


/**
 * \brief Base class for all implemented selectors. Traverses the AST for node selection
 * \ingroup Selector
 * \author InstRO
 */
class Selector {
public:
	Selector() :
			selectedNodes(0) {
	}
	virtual void traverseAST(SgNode* start) = 0; /**< \brief Starts the traversal of the AST. */
	size_t getId(); /**< \brief Returns a unique identifier per instance of each selector object. */
	int getNumberOfMarkedNodes(); /**< \brief Returns how many nodes in the AST have been selected by this selector. */

	/** \brief Fills a list with all sub-selectors of a selector */
	virtual void getSubSelectors(std::list<Selector*>& subSelectors) {
	}

	/**
	 * The method gets called from a marker, if it is attached to a node. If the decision is not based on markers which were found in the ast,
	 * the method simply returns an empty vector. Classes which base their selection decision on markers found in the ast, should return a
	 * vector with the markers, on which the decision was made.
	 * \todo 2013-10-26 JP: At the moment this method simply returns an empty vector.
	 * \todo What about the absence of markers?
	 */
	virtual std::vector<ASTAttributeContainer::SharedAttributePointerType> getSelectionDecisionMarkers();

protected:
	void select(SgNode* node); /**< \brief Tells the adapter builder to instrument this node */

private:
	int selectedNodes; // counter for selected nodes

	/** \brief Checks whether a marker is present or not */
	bool isSelected(SgNode* node);

	/** \brief Removes the marker to this selector instance */
	void deselect(SgNode* node);

	/** \brief returns the Marker of this
	 * \todo 2013-10-10 JP:Should it still return SelectionASTMarker* or SharedAttributePointerType? */
	virtual SelectionASTMarker * getSelectionMarker(SgNode* node);

	/**
	 * \brief Creates a simple SelectionMarker for indicating a node has been marked.
	 *  If you want your self defined selector to add an ast-marker you defined, overload this method to return a new
	 *  instance of your ast-marker. As the markers are internally stored using SmartPointers you should NOT DELETE the pointer.
	 */
	virtual SelectionASTMarker * createSelectionMarker();
};


/**
 * \brief Interface for everything that modifies the AST.
 *
 * This is the basis for AdapterBuilder and Transformer.
 *
 * \ingroup Astrewriter
 * \author Jan-Patrick Lehr
 */
class ASTReWriter {
public:
	/** Starts the modification pass over a set of SgNodes */
	virtual void startModificationPass() = 0;

	/**
	 * 2013-10-23 JP: The method can be used to check whether the internal state of the modifier is
	 * correct. It may be called before calling startModificationPass().
	 */
	virtual void validateInternalState() = 0;

	/**
	 * Sets the internally stored pointer to the SgProject.
	 * This method is called automatically when the ASTReWriter is registered in InstRO.
	 * If you call it explicitly make sure, that the project pointer is valid and is the one
	 * you want the Modifier to work on, anything else results in undefined behavior.
	 */
	virtual void setProject(SgProject* project) = 0;
};

class AdapterBuilder;
class Transformer;

/**
 * \brief Taking care of managing the selectors and adapterbuilders and of course the project itself.
 * \ingroup Instrumentor
 * \author InstRO
 */
class Instrumentor {
public:
	virtual void select() = 0; /**< \brief Lets every registered selector traverse the ast*/
	virtual void instrument() = 0; /**< \brief Lets the registered adapterbuilder traverse the ast*/
	virtual void transform() = 0; /**< for future purposes and consistency with UML */

	virtual void setProject(SgProject* project) = 0; /**\brief Set the project*/
	virtual SgProject* getProject() = 0; /**\brief get the project*/

	virtual void addSelector(Selector* sel) = 0; /** \brief Add a selector */
	virtual void removeSelector(Selector* sel) = 0; /** \brief remove the selector from the list of selectors */

	virtual void addAdapterBuilder(AdapterBuilder* builder) = 0; /** \brief Add aan adapter builder */
	virtual void removeAdapterBuilder(AdapterBuilder* builder) = 0; /** \brief remove the adapter builder from the list of adapter builders */

	virtual void addTransformer(Transformer* transformer) = 0;
	virtual void removeTransformer(Transformer* transformer) = 0;
};
}
#endif //INSTRUMENTOR_H__
