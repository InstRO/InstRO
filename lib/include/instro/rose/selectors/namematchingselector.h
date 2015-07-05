#ifndef INSTRUMENTOR_NAMEMATCHING_DEFINE
#define INSTRUMENTOR_NAMEMATCHING_DEFINE

#include "../common.h"
#include "../utility/matcher.h"
#include "../utility/marker.h"

namespace InstRO {

/**
 * The enum represents to which node the NamedMatcher should traverse upwards if it encounters
 * a match. Immediate say do not traverse upwards at all (XXX If we match SgName nodes, this doesn't
 * make sense at all!).
 * NextOuterExpression traverses the AST upwards until it finds the next outer SgExpression.
 * NextOuterFunctionDefinition traverses the AST upwards until the next SgFunctionDefinition node.
 * NextOuterStatement traverses upwards until the next statement node is encountered.
 * Parent simply traverses upwards one layer.
 * It is vital to keep in mind, that all subclasses of the mentioned (Expression, Statement) do
 * match this criterion, too.
 * \attention Please note that at the moment only IN_Immediate is supported
 */
enum IN_enum { IN_Immediate, IN_NextOuterExpression, IN_NextOuterFunctionDefinition, IN_NextOuterStatement, IN_Parent };

/**
 * \brief This class matches named entities of the AST against a list of specified strings.
 *
 *
 * In this context it is crucial to know which nodes are regarded to have a meaningful name.
 * We regard these nodes as named nodes:
 * - A variable or function defining definition node (this includes member functions)
 * - A function reference expression, a member function reference expression and a variable
 *   reference expression
 * - A label statement (as labels do have names)
 *
 * \throws InstroException if traversal is started before calling init()
 *
 * \ingroup Selector
 * \author Jan-Patrick Lehr
 */
class NameMatchingSelector : public PrePostOrderSelector {
 public:
	/** Standard constructor. Object needs to be initiated using the init() method */
	NameMatchingSelector();
	/** Standard constructor with possibilty to set verbosity. Object needs to be initiated using the init() method */
	NameMatchingSelector(bool verbose);
	/** Initializes the instance with the node-type which should be marked and which matching object should be used */
	void init(IN_enum nodetypeToMark, Matcher* matcherToUse, std::list<std::string>* listToMatchAgainst);

	/** Gets called before visiting the child nodes. Therefore this selector uses pre order traversal. */
	void preOrderVisit(SgNode* n);

	/** Retrieval of which strings matched a certain name will be implemented using an AST query */
	std::vector<std::string> retrieveMatches(SgNode* n);

	/** This method can be used to generate Strings from SgNodes.
	 * At the moment it will use only those SgNodes which are mentioned in the class documentation
	 * \todo move to some utility namespace / entity
	 */
	std::string toString(SgNode* n);

 private:
	IN_enum nodetypeToMark;											 // Where to save the ASTMarker
	Matcher* matchingObject;										 // Which matcher object should be used
	std::list<std::string>* listToMatchAgainst;	// List of strings to check against
	std::vector<int> lastMatchIds;							 // The list ids of the last matching
	bool verbose;																 // Is this object verbose?
	bool isInitialized;													 // for checking if the user called init method.

	// Fixme 2013-10-08 JP: which node gets selection attribute? How could this NOW be implemented?
	// Immediate, nextOuterExpression, nextOuterStatement, nextOuterFuncDef, parent, userdef (which is still missing atm)
	// 2013-10-08 JP: At the moment the immediate node is used.
	SelectionASTMarker* createSelectionMarker();
};
}
#endif	// INSTRUMENTOR_NAMEMATCHING_DEFINE
