#ifndef GEIMER_SELECTOR_H
#define GEIMER_SELECTOR_H

#include "../common.h"													// Basic definitions
#include "../selectors/namematchingselector.h"	// NameMatchingSelector
#include "geimerparser.h"												// Geimer configuration parsing and related things

namespace InstRO {

/**
 * This Selector demonstrates how newly (powerful) selectors can be build.
 * \ingroup Selector
 * \author Jan-Patrick Lehr
 */
class GeimerSelector : public AttributeSelector {
 public:
	/** \brief The constructor expects a Geimer specification file. */
	GeimerSelector(std::string const& specFile);	// With this constructor we could hide EVERY detail from the user!
	/** \brief The constructor expects a Geimer specification file. */
	GeimerSelector(const char* specFile);

	GeimerSelector(std::vector<std::string> whiteList);	// Is this constructor needed?
	~GeimerSelector();
	/** \brief Returns the ROSE project which is build from the specification file. */
	SgProject* getSpecProject();

	// XXX 2013-01-17 JPL: Maybe add another flag here whether its wild card or specific routine (leads to less string
	// compares)
	bool isRoutineInFileToBeInstrumented(
			std::string routinename,
			std::string filename);	// To retrieve whether a routine is in a certain file to be instrumented

	// Does this Selector need any public operation at all?
	virtual void traverseAST(SgNode* start);	// starts the traversal and invokes the subsequent selectors.

 protected:
	virtual void startNodeProcessing(SgNode* startNode);	// Starts the processing (gets called through traverseAST)

	virtual void processNode(SgNode* node);	// Called on every node, which holds a certain attribute

 private:
	/** JPL: returns the Marker of this (Should it still return SelectionASTMarker* or SharedAttributePointerType? ) */
	virtual SelectionASTMarker* getSelectionMarker(SgNode* node);	// Why is it virtual?
	virtual SelectionASTMarker* createSelectionMarker();

	// Needed for the black box approach
	GeimerConfigurationParser* gParser;
	GeimerAbstractCodeGenerator* gCodeGen;
	// ---

	GeimerConfigurationController gController;

	std::string specFile;
	std::vector<std::string> whiteList;
	NameMatchingSelector* nameMatchingSelector;	// Marks all nodes which
	SgProject* specProj;
	std::vector<SgNode*> t_nodePointers;	// XXX THIS IS A TEMPORARY NEEDED FOR DATA TRANSFER (Any other ideas?)
	std::vector<std::string> includes;
	bool insertIncludes;
};
}
#endif
