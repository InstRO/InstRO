#include <vector>
#include <list>
#include "rose.h"

namespace InstROLegacy {

/**
* \brief Base class for all implemented selectors. Traverses the AST for node selection
* \ingroup Selector
* \author InstRO
*/
class Selector {
 public:
	Selector() : selectedNodes(0) {}

	virtual ~Selector() {}

	virtual void traverseAST(SgNode* start) = 0; /**< \brief Starts the traversal of the AST. */
	size_t getId();								/**< \brief Returns a unique identifier per instance of each selector object. */
	int getNumberOfMarkedNodes(); /**< \brief Returns how many nodes in the AST have been selected by this selector. */

	/** \brief Fills a list with all sub-selectors of a selector */
	virtual void getSubSelectors(std::list<Selector*>& subSelectors) {}

	/**
	* The method gets called from a marker, if it is attached to a node. If the decision is not based on markers which
	* were found in the ast,
	* the method simply returns an empty vector. Classes which base their selection decision on markers found in the ast,
	* should return a
	* vector with the markers, on which the decision was made.
	* \todo 2013-10-26 JP: At the moment this method simply returns an empty vector.
	* \todo What about the absence of markers?
	*/
	virtual std::vector<ASTAttributeContainer::SharedAttributePointerType> getSelectionDecisionMarkers();

 protected:
	void select(SgNode* node); /**< \brief Tells the adapter builder to instrument this node */

 private:
	int selectedNodes;	// counter for selected nodes

	/** \brief Checks whether a marker is present or not */
	bool isSelected(SgNode* node);

	/** \brief Removes the marker to this selector instance */
	void deselect(SgNode* node);

	/** \brief returns the Marker of this
	* \todo 2013-10-10 JP:Should it still return SelectionASTMarker* or SharedAttributePointerType? */
	virtual SelectionASTMarker* getSelectionMarker(SgNode* node);

	/**
	* \brief Creates a simple SelectionMarker for indicating a node has been marked.
	*  If you want your self defined selector to add an ast-marker you defined, overload this method to return a new
	*  instance of your ast-marker. As the markers are internally stored using SmartPointers you should NOT DELETE the
	* pointer.
	*/
	virtual SelectionASTMarker* createSelectionMarker();
};
}