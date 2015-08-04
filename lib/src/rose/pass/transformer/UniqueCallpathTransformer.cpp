#include "instro/rose/pass/transformer/UniqueCallpathTransformer.h"

#include "instro/rose/deprecated_utility/callgraphmanager.h"

#include "instro/rose/utility/FunctionRenamer.h"

#include "instro/rose/core/RoseConstructSet.h"

#include <iostream>
#include <stack>
#include <queue>
#include <algorithm>

namespace {

/// A pair consisting of a pointer to a SgGraphNode and an integer which indicates its depth.
typedef std::pair<SgGraphNode*, int> NodeDepthEntry;

/// A functor to compare NodeDepthEntry instances by their depth using greater than.
struct NodeDepthComparator {
    bool operator()(const NodeDepthEntry &lhs, const NodeDepthEntry &rhs) {
        return lhs.second > rhs.second;
    }
};

}

using namespace InstRO;
using namespace InstRO::Rose;
using namespace InstRO::Rose::Transformer;

UniqueCallpathTransformer::UniqueCallpathTransformer(InstRO::Pass *pass)
    : UniqueCallpathTransformer(pass, nullptr, nullptr)
{

}

UniqueCallpathTransformer::UniqueCallpathTransformer(InstRO::Pass *pass, InstRO::Pass *root, InstRO::Pass *active)
    : RosePassImplementation(createChannelConfig(pass, root, active)), inputPass(pass), rootPass(root), activePass(active), manager(nullptr)
{

}

#if 0
UniqueCallpathTransformer::~UniqueCallpathTransformer()
{
    if (manager) {
        delete manager;
    }
}
#endif

InstRO::Core::ChannelConfiguration UniqueCallpathTransformer::createChannelConfig(InstRO::Pass *pass, InstRO::Pass *root, InstRO::Pass *active)
{
    std::vector<InstRO::Pass*> passes {pass};
    if (root) {
        passes.push_back(root);
    }
    if (active) {
        passes.push_back(active);
    }
    return InstRO::Core::ChannelConfiguration(passes.begin(), passes.end(), ::InstRO::Core::ConstructTraitType::CTFunction, ::InstRO::Core::ConstructTraitType::CTFunction);
}

UniqueCallpathTransformer::FunctionDeclarationSet UniqueCallpathTransformer::retrieveMarkedFunctions(InstRO::Pass *pass)
{
    InstRO::InfracstructureInterface::ConstructSetCompilerInterface cs (pass->getOutput());
    FunctionDeclarationSet result;
    result.reserve(cs.size());
    for (auto construct : cs) {
        auto rc = std::dynamic_pointer_cast<InstRO::Rose::Core::RoseConstruct>(construct);
        result.insert(isSgFunctionDeclaration(rc->getNode()));
    }

    return result;
    // std::cerr << SageInterface::get_name(node) << " is not a function declaration" << std::endl;

}

#if 0
void UniqueCallpathTransformer::transformNode(SgNode *node)
{
    SgFunctionDeclaration *funDecl = isSgFunctionDeclaration(node);

    if (!funDecl) {
        if (SgFunctionDefinition *funDef = isSgFunctionDefinition(node)) {
            funDecl = funDef->get_declaration();
        } else {
            std::cerr << "Skipping " << SageInterface::get_name(node) << ": UniqueCallpathTransformer accepts only function declarations or definitions as input" << std::endl;
            return;
        }
    }

    markedFunctions.insert(isSgFunctionDeclaration(funDecl->get_firstNondefiningDeclaration()));
}
#endif

void UniqueCallpathTransformer::execute() {
    // lazily initialize the call graph manager
    if (!manager) {
        manager = new CallGraphManager(SageInterface::getProject());
    }

    // use the main function as default root if none have been specified manually
    if (!rootPass) {
        roots.clear();
        roots.insert(SageInterface::findMain(SageInterface::getProject()));
    } else {
        roots = retrieveMarkedFunctions(rootPass);
    }

    // convert marked functions to call graph nodes
    InstRO::InfracstructureInterface::ConstructSetCompilerInterface inputCS (inputPass->getOutput());
    std::unordered_set<SgGraphNode*> markedNodes;
    markedNodes.reserve(inputCS.size());
    for (auto construct : inputCS) {
        auto rc = std::dynamic_pointer_cast<InstRO::Rose::Core::RoseConstruct>(construct);
        if (SgFunctionDeclaration *markedDecl = isSgFunctionDeclaration(rc->getNode())) {
            markedNodes.insert(manager->getCallGraphNode(markedDecl));
        } else if (SgFunctionDefinition *markedDef = isSgFunctionDefinition(rc->getNode())) {
            markedNodes.insert(manager->getCallGraphNode(markedDef->get_declaration()));
        } else {
            std::cerr << "Unsupported input node: " << SageInterface::get_name(rc->getNode()) << " (" << rc->getNode()->class_name() << ")" << std::endl;
        }
    }

    // initialize active nodes
    activeNodes.clear();
    if (activePass) {
        auto activeFunctions = retrieveMarkedFunctions(activePass);
        activeNodes.reserve(activeFunctions.size());
        for (SgFunctionDeclaration *activeFunction : activeFunctions) {
            activeNodes.insert(manager->getCallGraphNode(activeFunction));
        }
    }

    // find candidates for duplication and nodes which are targeted by a backwards directed edge
    NodeDepthMap candidates;
    NodeSet cycleNodes;
    findCandidates(markedNodes, candidates, cycleNodes);

    // use the found candidates to initialize a priority queue which sorts its entries with ascending depth:
    // nodes with a smaller depth will be processed before those with a larger depth
    std::priority_queue<NodeDepthEntry, std::vector<NodeDepthEntry>, NodeDepthComparator> work (candidates.begin(), candidates.end());
    NodeSet visited;
    NodeFunctionDeclarationMap duplicatedNodes;

    while (!work.empty()) {
        NodeDepthEntry entry = work.top();  // entry = (node, depth)
        work.pop();

        // get the associated function
        SgFunctionDeclaration *funDecl = manager->getFunctionDeclaration(entry.first);

        // a node may only be visited once or multiple definitions will be created
        if (visited.find(entry.first) != visited.end()) {
            std::cout << SageInterface::get_name(funDecl) << " has already been visited" << std::endl;
            continue;
        }
        visited.insert(entry.first);

        // safety check, in case a node which starts a cycle somehow manages to sneak in
        if (cycleNodes.find(entry.first) != cycleNodes.end()) {
            std::cout << "Ignore loop node" << std::endl;
            continue;
        }

        // debug output...
        std::cout << "Visiting " << SageInterface::get_name(funDecl) << " with depth " << entry.second << std::endl;

        // only active nodes may be transformed (if enabled)
        if (!activeNodes.empty() && activeNodes.find(entry.first) == activeNodes.end()) {
            std::cerr << "Cannot create unique call path because function " << SageInterface::get_name(funDecl) << " is not considered 'active'" << std::endl;
            continue;
        }

        // get the defining declaration
        funDecl = isSgFunctionDeclaration(funDecl->get_definingDeclaration());
        if (!funDecl || !funDecl->get_definition()) {
            funDecl = manager->getFunctionDeclaration(entry.first);
            std::cerr << "Cannot duplicate function " << SageInterface::get_name(funDecl) << " because no definition is available" << std::endl;
            continue;
        }

        // duplicate function
        std::cout << "Duplicating " << SageInterface::get_name(funDecl) << std::endl;
        duplicate(entry.first, duplicatedNodes);

        // mark successors (functions called by the function which has just been duplicated) for duplication,
        for (SgGraphNode *succ : manager->getChildrenSet(entry.first)) {
            work.push(std::make_pair(succ, entry.second + 1));
        }
    }

    // invalidate the call graph
    delete manager;
    manager = nullptr;

}

void UniqueCallpathTransformer::findCandidates(const NodeSet &markedNodes, NodeDepthMap &candidates, NodeSet &cycleNodes)
{
    NodeList localPath;

    // start a depth first search starting at each root and update candidates if a marked node appears
    // at the end of the current path through the call graph
    for (SgFunctionDeclaration* rootFunction : roots) {
        SgGraphNode* rootNode = manager->getCallGraphNode(rootFunction);

        // initialize data structures
        localPath.clear();

        // start traversal at the root node at depth 0
        std::deque<SgGraphNode*> workStack;
        std::deque<int> depthStack;
        workStack.push_back(rootNode);
        depthStack.push_back(0);

        while (!workStack.empty()) {
            SgGraphNode *node = workStack.back();
            workStack.pop_back();
            auto depth = depthStack.back();
            depthStack.pop_back();

            // check for cycle: we cannot simply check whether we have already visited this node
            // because a node may be visited more than once
            if (cycleNodes.find(node) != cycleNodes.end()) {
                // skip cycles which have already been found
                // alternatively, one could just remove the node from the workStack once a cycle has been found for the first time
                continue;
            }
            // a node is considered to start a cycle if it is contained in the current working stack
            if (std::find(workStack.begin(), workStack.end(), node) != workStack.end()) {
                std::cerr << "Found cycle at " << SageInterface::get_name(manager->getFunctionDeclaration(node)) << std::endl;
                cycleNodes.insert(node);
                continue;
            }

            // append node to the local path
            localPath.push_back(node);

            // update candidates if the current node is marked
            if (markedNodes.find(node) != markedNodes.end()) {
                updateCandidates(candidates, localPath);
            }

            // add successors / children to working stack
            auto successors = manager->getChildrenSet(node);
            for (SgGraphNode* succ : successors) {
                // check for recursion because it is not detected by the cycle check above
                if (succ == node) {
                    std::cerr << "Found recursion at " << SageInterface::get_name(manager->getFunctionDeclaration(node)) << std::endl;
                    cycleNodes.insert(node);
                } else {
                    workStack.push_back(succ);
                    depthStack.push_back(depth + 1);
                }
            }

            // the local path must be reset / trimmed if we have found an end and still have work to do
            if (successors.empty() && !workStack.empty()) {
                // discard entries which have a depth greater than or equal to the depth of the node that is visited next
                localPath.erase(localPath.begin() + depthStack.back(), localPath.end());
            }
        }
    }

    // remove cycle nodes from the found candidates
    for (SgGraphNode *cn : cycleNodes) {
        candidates.erase(cn);
    }
}

void UniqueCallpathTransformer::updateCandidates(NodeDepthMap &candidates, const NodeList &path)
{
    std::cout << "Found path: ";
    for (NodeDepthMap::mapped_type i = 0; i < path.size(); ++i) {
        // check whether node is a candidate (has more than one predecessor)
        if (manager->getParentSet(path[i]).size() > 1) {
            auto candidateIter = candidates.find(path[i]);
            if (candidateIter != candidates.end()) {
                // update max call depth
                candidateIter->second = std::max(i, candidateIter->second);
            } else {
                // initialize max call depth
                candidates[path[i]] = i;
            }
        }

        std::cout << SageInterface::get_name(manager->getFunctionDeclaration(path[i])) << " ";
    }
    std::cout << std::endl;
}

std::string UniqueCallpathTransformer::generateCloneName(SgFunctionDeclaration *caller, SgFunctionDeclaration *callee)
{
    return callee->get_name().getString() + "_ucp_" + caller->get_name().getString();
}

void  UniqueCallpathTransformer::duplicate(SgGraphNode *node, NodeFunctionDeclarationMap &duplicatedNodes)
{
    // retrieve the defining function declaration for the node
    SgFunctionDeclaration *funDecl = isSgFunctionDeclaration(manager->getFunctionDeclaration(node)->get_definingDeclaration());

    // if a parent has already been duplicated, the clones must be used instead as they are not picked up be the 'old' call graph
    auto parents = manager->getParentSet(node);
    std::vector<SgFunctionDeclaration*> callingFunctions;
    callingFunctions.reserve(parents.size());

    for (SgGraphNode* parentNode : parents) {
        auto range = duplicatedNodes.equal_range(parentNode);

        if (range.first == duplicatedNodes.end()) {
            // parent has not been duplicated
            // the function definition of the parent should always be available - otherwise it would have been difficult to construct the call graph...
            SgFunctionDeclaration *definingParentDecl = isSgFunctionDeclaration(manager->getFunctionDeclaration(parentNode)->get_definingDeclaration());
            callingFunctions.push_back(definingParentDecl);
        } else {
            // parent has been duplicated, add all created duplicates
            std::for_each(range.first, range.second,
                          [&] (const NodeFunctionDeclarationMap::value_type &entry) { callingFunctions.push_back(entry.second); });
        }
    }

    // create a duplicate of the function for each function calling it
    for (SgFunctionDeclaration *callingFunDecl : callingFunctions) {
        FunctionRenamer renamingProvider (funDecl);
        // look for function calls to the function to be duplicated inside the definition of the calling parent function
        renamingProvider.findFunctionCalls(std::vector<SgNode*> {callingFunDecl});

        // clone the function and insert the clone after the original
        SgFunctionDeclaration *clonedFunction = cloneFunction(funDecl, generateCloneName(callingFunDecl, funDecl), funDecl->get_scope());
        SageInterface::insertStatementAfter(funDecl, clonedFunction);
        // rename the previously found calls to the original so that they call the clone
        renamingProvider.redirectFunctionCalls(clonedFunction);

        // remember the duplicate for successors of the node which might have to be duplicated
        duplicatedNodes.insert(std::make_pair(node, clonedFunction));

        InstRO::InfracstructureInterface::ConstructSetCompilerInterface output (&outputCS);
        // add the duplicate to the output of the transformer
        output.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(clonedFunction->get_definition()));
        // add the redirected function calls to the output
        for (SgFunctionCallExp *funCall : renamingProvider.getFoundFunctionCalls()) {
            output.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(funCall));
        }
    }
}

SgFunctionDeclaration* UniqueCallpathTransformer::cloneFunction(SgFunctionDeclaration* originalFunction, const std::string &cloneName, SgScopeStatement *scope)
{
    // build the list of function parameters
    const SgInitializedNamePtrList& originalArgs = originalFunction->get_args();
    SgFunctionParameterList *parameterList = SageBuilder::buildFunctionParameterList();
    for(const SgInitializedNamePtr arg : originalArgs)
    {
        SageInterface::appendArg(parameterList, SageBuilder::buildInitializedName(arg->get_name(), arg->get_type()));
    }

    SgExprListExp *decorators = SageInterface::deepCopy(originalFunction->get_decoratorList());

    // check if there is a non defining function declaration available for building the defining one
    SgFunctionDeclaration *nondefiningClonedFunction;
    SgFunctionSymbol *funcSymbol = SageInterface::lookupFunctionSymbolInParentScopes(cloneName, originalFunction->get_type(), scope);
    if (funcSymbol) {
        nondefiningClonedFunction = funcSymbol->get_declaration();
    } else {
        nondefiningClonedFunction = SageBuilder::buildNondefiningFunctionDeclaration(cloneName, originalFunction->get_orig_return_type(), parameterList, scope, decorators);
        nondefiningClonedFunction->set_exceptionSpecification(originalFunction->get_exceptionSpecification());
    }

    // build a defining function declaration for the clone using a undefined one (ROSE...)
    SgFunctionDeclaration *clonedFunction = SageBuilder::buildDefiningFunctionDeclaration(cloneName, originalFunction->get_orig_return_type(),
                                                         parameterList, scope, decorators, false,
                                                         isSgFunctionDeclaration(nondefiningClonedFunction->get_firstNondefiningDeclaration()));

    // copy the exception specification
    clonedFunction->set_exceptionSpecification(originalFunction->get_exceptionSpecification());
    SgBasicBlock *clonedBody = SageInterface::deepCopy(originalFunction->get_definition()->get_body());
    SageInterface::replaceStatement(clonedFunction->get_definition()->get_body(), clonedBody);

    return clonedFunction;
}



