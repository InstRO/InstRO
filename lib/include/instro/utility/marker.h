#ifndef INSTRUMENTOR_ASTATTRIBUTES_H__
#define INSTRUMENTOR_ASTATTRIBUTES_H__

/*
 * In order to use smart pointer facilities we use either the boost implementation
 * of C++ TR1 or the native C++0x implementation. Therefore we need some preprocessor.
 */

// Code taken from: www.codesynthesis.com/~boris/blog/2010/05/24/smart-pointers-in-boost-tr1-cxx-x0/

#include <cstddef> // __GLIBCXX__, _HAS_TR1
//
// GNU C++ or Intel C++ using libstd++
//
#if defined (__GNUC__) && __GNUC__ >= 4 && defined (__GLIBCXX__)
#	include <tr1/memory>
//
//IBM XL C++
//
#elif defined (__clC__) && __xlC__ >= 0x0900
#	define __IBMCPP_TR1__
#	include <memory>
//
// VC++ or Intel C++ using VC++ standard library
//
#elif defined (_MSC_VER) && (_MSC_VER == 1500 && defined (_HAS_TR1) || _MSC_VER > 1500)
#	include <memory>
//
// Boost fall-back
//
#else
#	include <boost/tr1/memory.hpp>
#endif

#include <rose.h> // XXX This include necessary as it is included in instrumentor.h....
#include "../exception/instroexception.h"

namespace InstRO{

/* Retrieval name for the container used to store the InstRo markers. */
#define ROSE_AST_CONTAINER_ATTRIB_NAME "INSTROContainerAttrib"

class SelectionASTMarker; // JPL: Forward Declaration
class Selector; // JPL: Forward declaration
/**
 * \brief stores information at AST nodes, if they get selected
 *
 *
 * The ASTAttributeContainer stores a hash map to retrieve if a specific selector instance marked
 * this node. It provides static methods for creation and retrieval of the container at a node n.
 * During traversal a Selector could ask the container why a specific node was selected using the stored
 * markers.
 * \ingroup Markers
 * \author Jan-Patrick Lehr
 */
class ASTAttributeContainer : public AstAttribute{
public:

    typedef std::tr1::shared_ptr<SelectionASTMarker> SharedAttributePointerType;
    /* Returns the ast attribute container at a node or NULL if no container attached */
    static ASTAttributeContainer* hasASTAttributeContainer(SgNode* node);
    /* If no container object exists creates new one, else retrieves existing and returns pointer to object */
    static ASTAttributeContainer* getASTAttributeContainer(SgNode* n);
    /* Constructor */
    ASTAttributeContainer();
    /* Destructor */
    ~ASTAttributeContainer();
    /* Sets the attribute to the map */
    void setSelAttrib(size_t selId, SharedAttributePointerType attrib);
    /* Removes an attribute by the selectors id */
    void remSelAttrib(size_t selId);
    /* Retrieves an attribute by the selectors id */
    SharedAttributePointerType getSelAttrib(size_t selId);
    /* Returns true iff the container holds an attribute to corresponding selector instance */
    bool hasSelAttrib(size_t selId);

private:
    /* Holds a map between selectors id and the stored attribute for all selectors which selected this node */
    std::map<size_t, SharedAttributePointerType> selectorMarkerMap;
};

/**
 * \brief This is a base class to reflect that a node has been selected by a selector instance.
 *
 *
 * As a name convention the selector name is used as a prefix in derived classes.
 * \ingroup Markers
 * \author Jan-Patrick Lehr
 */
class SelectionASTMarker{
public:
	virtual ~SelectionASTMarker() {}

    void select(Selector* sel);		/**< \brief Selects the attached node to be instrumented explicitely by the given Selector */
    Selector* getSelectingSelector();
    std::vector<ASTAttributeContainer::SharedAttributePointerType > getSelectionBase();

private:
    /* This method can be re-implemented in derived classes */
    virtual std::vector<ASTAttributeContainer::SharedAttributePointerType > selectionBase();

    Selector* selectingSelector; // holds a pointer to the selector which selected the node
    std::vector<ASTAttributeContainer::SharedAttributePointerType > markersLedToSelection;
};

/**
 * \brief Reflects that a NameMatchingSelector instance marked this node to be instrumented.
 *
 *
 * The NameMatchingSelectionASTMarker holds a list of matches which led to selection of the
 * AST node.
 * \ingroup Markers
 * \author Jan-Patrick Lehr
 */
class NameMatchingSelectionASTMarker : public SelectionASTMarker{
public:
    NameMatchingSelectionASTMarker(Selector* sel);
    /** Add a string to the list of matching strings. */
    void addMatchToList(std::string str);
    /** Retrieve the list of string which led to selection of the node */
    std::list<std::string> getMatches();

private:
    std::list<std::string> matches; // holds all strings which lead to selection
};

/**
 * \brief Reflects that a BlackWhiteListSelector instance marked this node to be instrumented
 *
 *
 * Using the getSelectingSelector method it is possible to track which matches led to
 * selection in black and white list.
 * \ingroup Markers
 * \author Jan-Patrick Lehr
 */
class BWSelectionASTMarker : public SelectionASTMarker{
public:
    BWSelectionASTMarker(Selector* selectingSelector);
    Selector* getSelectingSelector();
    // XXX 2013-10-08 can this be removed?
    void addIdsToList(std::vector<int> idsToAddToList);
    std::vector<int> getListOfMatchingIds();
private:
    std::vector<int> matchingIdsList;
    Selector* selectingSelector;
};

/**
 * \brief Reflects the selection through a CompoundSelector.
 * \todo Add a list of selectors which led to selecting this node, or think if its more accurate to ask the selector instance again!
 * \ingroup Markers
 * \author Jan-Patrick Lehr
 */
class CompoundSelectionASTMarker : public SelectionASTMarker{
public:
    //void select(Selector* sel); // For consistency remove this (as it is implemented in SelectionAstMarker.
    Selector* getSelectingSelector();

private:
    Selector* selectingSelector; // Pointer to selecting selector

};

/**
 *  \brief This class represents the Selection AST Marker a Geimer Selector decorates the Ast with.
 *  The idea is to have attributes in the marker to store the various constructs Geimer et al, in their paper, identified as the building
 *  blocks. (Variable Declaration, Function Entry / Exit, Include Statements, Abort and Init Statements)
 *  \ingroup Markers
 *  \author Jan-Patrick Lehr
 */
class GeimerSelectionASTMarker : public SelectionASTMarker{
public:
    std::vector<std::string> getIncludeFilenames();
    void setIncludeFiles(std::vector<std::string> includeFiles);
    void setSpecNodes(std::vector<SgNode*> nodes);
    std::vector<SgNode*> getSpecNodes();

private:
    /* data fields for some container, which hold the information */
    std::vector<SgNode*> specNodes;
    std::vector<std::string> includeFiles;
};
}
#endif // INSTRUMENTOR_ASTATTRIBUTES_H__
