#ifndef INSTRO_ROSE_UNIQUECALLPATHTRANSFORMER_H
#define INSTRO_ROSE_UNIQUECALLPATHTRANSFORMER_H

#include "instro/Pass.h"
#include "instro/rose/core/RosePassImplementation.h"
#include "instro/tooling/ExtendedCallGraph.h"

#include <set>
#include <unordered_set>
#include <unordered_map>

class SgNode;
class SgFunctionDeclaration;
class SgGraphNode;
class SgScopeStatement;

namespace InstRO {

namespace Rose {
namespace Transformer {

/// \brief Creates a unique call path for all marked input nodes by duplicating functions of their call path.
///
/// The Transformer accepts both defining function declarations and function definitions as input
/// and outputs the function definitions of the created duplicates as well as the function calls that have been changed.
/// The main input Selector specifies the functions whose call graph should be unique after the transformation.
/// Two additional Selector instances may be supplied:
/// \li A \c root Selector which indicates the roots of the call graph. Those functions are the possible
/// starting points of the call paths. If none is set, the main function (entry point) is used.
/// \li An \c active Selector which specifies a white list of functions which are allowed to be duplicated.
/// If none is set, all functions are considered to be active.
///
/// The algorithm is split into two main phases:
/// \li In the first phase all possible initial candidates for duplication and their maximum depth
/// (= distance from a root) is determined. In addition, a check for cycles / recursion is realized in this phase,
/// as it is not possible to handle these with duplication.
/// \li In the second phase the actual duplication is done in order of the depth: Functions with a lower depth
/// are duplicated before functions with a higher depth. If a function has been duplicated, its successors
/// in the call graph are considered for duplication as well.
/// A function is duplicated for each predecessor in the call graph and the function calls in each one are redirected
/// to call the appropriate duplicate instead of the original.
///
/// \note This transformation relies on the availability of function definitions for almost all inspected functions.
///
/// \author Simon Reuß, Jan-Patrick Lehr
class RoseUniqueCallpathTransformer : public RosePassImplementation {
 public:
	enum class PassType { InputPT, RootPT, ActivePT };
	typedef std::map<PassType, int> InputMapping;

	/// \brief Constructs a new UniqueCallpathTransformer that expects only a single input channel specifying the
	/// functions for which a unique call path should be created.
	RoseUniqueCallpathTransformer();

	/// \brief Constructs a new UniqueCallpathTransformer with an explicit mapping of the expected input channels.
	/// \arg mapping Maps the pass type to the expected input channel
	RoseUniqueCallpathTransformer(InputMapping mapping);

	virtual ~RoseUniqueCallpathTransformer();

	virtual void execute() override;

 protected:
	InputMapping mapping;

	/// Generates the new name for the cloned function.
	/// \arg caller The function calling the duplicate
	/// \arg callee The original function
	virtual std::string generateCloneName(SgFunctionDeclaration *caller, SgFunctionDeclaration *callee);

 private:
	typedef std::unordered_map<InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode *, int> NodeDepthMap;
	typedef std::unordered_set<InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode *> NodeSet;
	typedef std::unordered_set<SgFunctionDeclaration *> FunctionDeclarationSet;
	typedef std::vector<InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode *> NodeList;
	typedef std::unordered_multimap<InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode *, SgFunctionDeclaration *>
			NodeFunctionDeclarationMap;

	InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph *callGraph;

	NodeSet rootNodes;
	NodeSet activeNodes;

	NodeSet retrieveInputNodes(PassType passType);
	NodeSet retrieveInputNodes(int channel);
	InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode *getMainFunctionNode();

	NodeSet getSuccessorFunctions(InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode *node);
	NodeSet getPredecessorFunctions(InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode *node);
	SgFunctionDeclaration *getFunDeclFromNode(InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode *node);

	void addNodeToCS(InstRO::InfrastructureInterface::ConstructSetCompilerInterface &csci, SgNode *node);

	void findCandidates(const NodeSet &markedNodes, NodeDepthMap &candidates, NodeSet &cycleNodes);
	void updateCandidates(NodeDepthMap &candidates, const NodeList &path);

	void duplicate(InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode *node,
								 NodeFunctionDeclarationMap &duplicatedNodes);

	SgFunctionDeclaration *cloneFunction(SgFunctionDeclaration *originalFunction, const std::string &cloneName,
																			 SgScopeStatement *scope);
};
}
}
}

#endif	// INSTRO_ROSE_UNIQUECALLPATHTRANSFORMER_H
