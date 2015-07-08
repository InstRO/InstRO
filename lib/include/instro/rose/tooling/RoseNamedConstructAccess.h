#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
#include <stack>

#include "instro/core/Helper.h"

#include "instro/core/ConstructSet.h"
#include "instro/tooling/NamedConstructAccess.h"

#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Tooling {
namespace NamedConstructAccess {
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
* \author Jan-Patrick Lehr, Christian Iwainsky
*/
class NameMatchingASTTraversal : public AstPrePostProcessing {
 protected:
	InstRO::Core::ConstructSet cs;

 public:
	NameMatchingASTTraversal() : csci(&cs) {}
	bool relevantNode(SgNode* node);
	// NameMatchingASTTraversal(InstRO::Tooling::NamedConstructAccess::Matcher & m):matchingObject(&m){}
	void reset() {
		preOrderMatch = false;
		postOrderMatch = false;
	}

	void setMatchMax() {
		continueDescend = false;
		preOrderMatch = true;
		postOrderMatch = false;
	};
	void setMatchMin() {
		postOrderMatch = true;
		preOrderMatch = false;	// continueDescend does not apply
	};
	void setMatchAll() {
		continueDescend = true;
		preOrderMatch = true;
		postOrderMatch = false;
	};

	std::unique_ptr<InstRO::Core::ConstructSet> matchUserIdentifyer(
			::InstRO::Tooling::NamedConstructAccess::Matcher* matcher, SgProject* proj) {
		matchingObject = matcher;
		cs.clear();
		traverseAST(proj);
		return std::make_unique<InstRO::Core::ConstructSet>(cs);
	}
	std::unique_ptr<InstRO::Core::ConstructSet> matchUserTextString(
			::InstRO::Tooling::NamedConstructAccess::Matcher* matcher, SgProject* proj) {
		matchingObject = matcher;
		cs.clear();
		traverseAST(proj);
		return std::make_unique<InstRO::Core::ConstructSet>(cs);
	}
	std::unique_ptr<InstRO::Core::ConstructSet> matchCode(::InstRO::Tooling::NamedConstructAccess::Matcher* matcher,
																												SgProject* proj) {
		matchingObject = matcher;
		cs.clear();
		traverseAST(proj);
		return std::make_unique<InstRO::Core::ConstructSet>(cs);
	}

	//	virtual void selectionBegin( 			SgProject*					project); /**< \brief Gets called before the selection
	//process starts. Might be used for initialization */
	//	virtual void selectionEnd(			SgProject*					project); /**< \brief Gets called after the selection process
	//has finished. Might be used for cleaning up */
	/** \brief Gets called before visiting the children of a node. Either this or postOrderVisit should be implemented.
	 * Otherwise the selector is useless */
	virtual void preOrderVisit(SgNode* n);
	/** \brief Gets called after visiting the children of a node. Either this or preOrderVisit should be implemented.
	 * Otherwise the selector is useless */
	virtual void postOrderVisit(SgNode* n);

	void traverseAST(SgNode* start) { /**< \brief Starts the traversal of the AST. */
																		//		this->selectionBegin(SageInterface::getEnclosingNode<SgProject>(start, true));
		traverse(start);
		//		this->selectionEnd(SageInterface::getEnclosingNode<SgProject>(start, true));
	}
	::InstRO::Core::ConstructSet getConstructs() { return cs; }

	/** This method can be used to generate Strings from SgNodes.
	 * At the moment it will use only those SgNodes which are mentioned in the class documentation
	 * \todo move to some utility namespace / entity
	 */
	std::string toString(SgNode* n);

 private:
	std::stack<SgNode*> nodeTracker;
	bool preOrderMatch,	// match first, if unsuccessfull decend afterwards
			postOrderMatch,	// descend first, match afterwards
			continueDescend;
	InstRO::InfracstructureInterface::ConstructSetCompilerInterface csci;
	//	IN_enum nodetypeToMark;																						 // Where to save the ASTMarker
	::InstRO::Tooling::NamedConstructAccess::Matcher* matchingObject;	// Which matcher object should be used
	// std::list<std::string>* listToMatchAgainst; // List of strings to check against
	//	std::vector<int> lastMatchIds; // The list ids of the last matching
	bool verbose;				 // Is this object verbose?
	bool isInitialized;	// for checking if the user called init method.
	void select(SgNode* node);

	// Fixme 2013-10-08 JP: which node gets selection attribute? How could this NOW be implemented?
	// Immediate, nextOuterExpression, nextOuterStatement, nextOuterFuncDef, parent, userdef (which is still missing atm)
	// 2013-10-08 JP: At the moment the immediate node is used.
	// SelectionASTMarker* createSelectionMarker();
};

class RoseNamedConstructAccess : public ::InstRO::Tooling::NamedConstructAccess::NamedConstructAccess {
 protected:
	NameMatchingASTTraversal traversal;
	SgProject* project;

 public:
	RoseNamedConstructAccess(SgProject* proj) : project(proj) {
		std::cout << "Constructing RoseNamedConstructAccess" << std::endl;
	};
	std::unique_ptr<InstRO::Core::ConstructSet> getConstructsByIdentifyerName(
			::InstRO::Tooling::NamedConstructAccess::Matcher& matcher) override {
		std::cout << "Searching the ROSE AST for suitable strings\n" << std::endl;
		traversal.reset();
		traversal.setMatchMin();
		return traversal.matchUserIdentifyer(&matcher, project);
	};
	std::unique_ptr<InstRO::Core::ConstructSet> getConstructsByUserTextStringMatch(
			::InstRO::Tooling::NamedConstructAccess::Matcher& matcher) override {
		traversal.reset();
		traversal.setMatchMin();
		return traversal.matchUserTextString(&matcher, project);
	};
	/* Not implemented in current instro
	Core::ConstructSet getConstructsByCodeMatch(::InstRO::Tooling::NamedConstructAccess::Matcher & matcher) override{
		traversal.reset();
		traversal.setMatchMin();
		return traversal.matchCode(&matcher, project);
	};*/
};
}
}
}
}
