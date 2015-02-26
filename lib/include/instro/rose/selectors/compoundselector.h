
#ifndef COMPOUNDSELECTOR_H_
#define COMPOUNDSELECTOR_H_

#include "../common.h"

namespace InstRO {

/**
 * \brief The CompoundSelector class serves as a base class for all selectors which operate on two or more sub-selectors.
 * Therefore no traversal order has to be regarded.
 *
 * \todo implement the functionality of applying some operation to the selectors
 * \ingroup Selector
 * \author Jan-Patrick Lehr, Roman Neß
 */
class CompoundSelector : public AttributeSelector{
public:
	/** RN: always initialize both, subSelectors and the inversion list */
	CompoundSelector(std::vector<Selector*> selectors, std::vector<bool> inversions);
	CompoundSelector(std::vector<Selector*> selectors); // XXX 2013-10-08 JP: Why this constructor?

    /** Starts node processing at node start. */
    virtual void traverseAST(SgNode *start);
    /** This method applies the specific operation to each node in the set of nodes based on the existence of selector markers */
    virtual void applyOperation(SgNode* n); // can go to protected / private
    /** Starts the processing of the nodes starting at startNode */
    virtual void startNodeProcessing(SgNode* startNode); // JPL: Goes to protected

    void getSubSelectors(std::list<Selector*>& subSelectorList);

protected:
    CompoundSelector();

    std::vector<Selector*> subSelectors;

    std::vector<bool> invSelectors; // list of bools if selectors at different position should be inverted


    /** inherited from AttributeSelector. This is called on every found node */
    virtual void processNode(SgNode* n);

private:
    SelectionASTMarker* createSelectionMarker();
    Rose_STL_Container<SgNode*> nodesList; /** holds a list of nodes to process */
};

/**
 * \brief Selector which operates on two or more selectors and provides some set-operations
 *
 *
 * This selector operates on two or more selectors and provides the functionality of union
 * - which selects a node as soon as a marker of one of the provided selectors is found - and
 * intersection - which selects a node only if markers from all provided selectors are found.
 * With the invList the user can invert the meaning of found markers from the corresponding
 * selector.
 * \ingroup Selector
 * \author Jan-Patrick Lehr
 */
class SetSelector : public CompoundSelector{
public:
    /** sels is a stl vector of Selector obj pointers. an stl vector of bools indicates if each selector is to be regarded as inverted */
    SetSelector(std::vector<Selector*> sels, std::vector<bool> invList);

    /** Performs the evaluation of an n-ary intersection (returns true if markers from each selector (or its inverted meaning) are at the node */
    bool evalNaryIntersection(ASTAttributeContainer* selCon); // n-ary intersection
    /** Performs an n-ary union. So returns true if at least one selector left a marker at a specific node. */
    bool evalNaryUnion(ASTAttributeContainer* selCon); // n-ary union

private:
};

/**
 * \brief Evaluates basic boolean operations (intersection, union, implication, xor) on two selectors
 *
 *
 * The BooleanCompoundSelector implements the basic boolean operations (and, or, xor, implication)
 * on two selectors. Therefore it traverses the AST (search-based) and selects all nodes where the
 * specified boolean operation evaluates to true.
 * The evaluation considers the two selectors as boolean variables at each node with the semantics,
 * that a missing marker serves as the value "false" and a present marker as the value "true".
 * \ingroup Selector
 * \author Jan-Patrick Lehr
 */
class BooleanCompoundSelector : public CompoundSelector{
public:
	/** Is used to determine the boolean operation an instance of the BooleanCompoundSelector should apply */
    enum OP_Enum {BIN_INTERSECTION, BIN_UNION, BIN_IMPLICATION, BIN_XOR};

    /** Have this constructor here in order to force the user to use two selectors */
    BooleanCompoundSelector(Selector* sel1, bool invSel1, Selector* sel2, bool invSel2, OP_Enum operation);
    /** Starts node processing */
    virtual void traverseAST(SgNode* start);
    virtual void startNodeProcessing(SgNode *startNode); // starts the processing at the startnode (e.g. the SgProject)
    virtual void processNode(SgNode *n); // gets called for every node to process.


protected:
    /*  force the user to use the five parameter constructor */
    BooleanCompoundSelector() { }

    virtual SelectionASTMarker* createSelectionMarker();

    bool evalLogicalAnd(ASTAttributeContainer* selCon); // and
    bool evalLogicalOr(ASTAttributeContainer* selCon); // or
    bool evalLogicalImplication(ASTAttributeContainer* selCon); // a->b
    bool evalLogicalXor(ASTAttributeContainer* selCon); // xor

private:

    OP_Enum operation; // Operation for this instance
};
}

#endif /* COMPOUNDSELECTOR_H_ */
