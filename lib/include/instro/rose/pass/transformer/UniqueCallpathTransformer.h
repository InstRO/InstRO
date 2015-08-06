#ifndef INSTRO_ROSE_UNIQUECALLPATHTRANSFORMER_H
#define INSTRO_ROSE_UNIQUECALLPATHTRANSFORMER_H

#include "instro/core/Pass.h"
#include "instro/rose/core/RosePassImplementation.h"

#include <set>
#include <unordered_set>
#include <unordered_map>

class SgFunctionDeclaration;
class SgGraphNode;
class SgScopeStatement;

namespace InstRO {

class CallGraphManager;

namespace Rose {
namespace Transformer {

/// \brief Creates a unique call path for all marked input nodes by duplicating functions of their call path.
///
/// The Transformer accepts both defining function declarations and function definitions as input
/// and outputs the function declarations of the created duplicates.
/// The main input selector specifies the functions whose call graph should be unique after the transformation.
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
/// \ingroup Transformer
/// \author Simon Reuß
class UniqueCallpathTransformer : public RosePassImplementation {

public:
    UniqueCallpathTransformer(InstRO::Pass *pass);
    UniqueCallpathTransformer(InstRO::Pass *pass, InstRO::Pass *root, InstRO::Pass *active);

    ~UniqueCallpathTransformer();

    virtual void init() override {};
	virtual void execute() override;
	virtual void finalize() override {};
	virtual void releaseOutput() override { outputCS.clear(); };
	virtual InstRO::Core::ConstructSet *getOutput() override { return &outputCS; }

    /// Sets the Selector which provides the root nodes (functions) of the call graph.
    /// \arg selector The selector to use or \c NULL to fall back to the default behavior of using the main function
    //void setRootSelector(Selector *selector);
    /// Sets the Selector which provides the active nodes (functions) which are allowed to be duplicated.
    /// \arg selector The selector to use or \c NULL to fall back to the default behavior of allowing all nodes to be duplicated
    //void setActiveSelector(Selector *selector);

protected:
    InstRO::Pass *inputPass;
    InstRO::Pass *rootPass;
    InstRO::Pass *activePass;

    InstRO::Core::ConstructSet outputCS;

    /// Generates a new name for the cloned function.
    /// \arg caller The function calling the duplicate
    /// \arg callee The original function
    virtual std::string generateCloneName(SgFunctionDeclaration *caller, SgFunctionDeclaration *callee);

private:
    typedef std::unordered_map<SgGraphNode*, int> NodeDepthMap;
    typedef std::unordered_set<SgGraphNode*> NodeSet;
    typedef std::unordered_set<SgFunctionDeclaration*> FunctionDeclarationSet;
    typedef std::vector<SgGraphNode*> NodeList;
    typedef std::unordered_multimap<SgGraphNode*, SgFunctionDeclaration*> NodeFunctionDeclarationMap;

    CallGraphManager *manager;

    NodeSet rootNodes;
    NodeSet activeNodes;

    InstRO::Core::ChannelConfiguration createChannelConfig(InstRO::Pass *pass, InstRO::Pass *root, InstRO::Pass *active);

    NodeSet retrieveInputNodes(InstRO::Pass *pass);

    void findCandidates(const NodeSet &markedNodes, NodeDepthMap &candidates, NodeSet &cycleNodes);
    void updateCandidates(NodeDepthMap &candidates, const NodeList &path);

    void duplicate(SgGraphNode *node, NodeFunctionDeclarationMap &duplicatedNodes);

    SgFunctionDeclaration* cloneFunction(SgFunctionDeclaration* originalFunction, const std::string &cloneName, SgScopeStatement *scope);

};

}
}
}

#endif // INSTRO_ROSE_UNIQUECALLPATHTRANSFORMER_H
