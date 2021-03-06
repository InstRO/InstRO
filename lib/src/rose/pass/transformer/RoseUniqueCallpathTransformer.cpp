#include "../../../../include/instro/rose/pass/transformer/RoseUniqueCallpathTransformer.h"
#include "instro/rose/utility/FunctionRenamer.h"

#include "instro/Instrumentor.h"
#include "instro/core/Singleton.h"
#include "instro/rose/core/RoseConstructSet.h"
#include "instro/utility/Logger.h"

#include <stack>
#include <queue>
#include <algorithm>

using namespace InstRO::Tooling::ExtendedCallGraph;

namespace {

/// A pair consisting of a pointer to a ExtendedCallGraphNode and an integer which indicates its depth.
typedef std::pair<ExtendedCallGraphNode *, int> NodeDepthEntry;

/// A functor to compare NodeDepthEntry instances by their depth using greater than.
struct NodeDepthComparator {
	bool operator()(const NodeDepthEntry &lhs, const NodeDepthEntry &rhs) { return lhs.second > rhs.second; }
};
}

using namespace InstRO;
using namespace InstRO::Core;
using namespace InstRO::Rose;
using namespace InstRO::Rose::Transformer;

RoseUniqueCallpathTransformer::RoseUniqueCallpathTransformer()
		: RosePassImplementation(), mapping({std::make_pair(PassType::InputPT, 0)}), callGraph(nullptr) {}

RoseUniqueCallpathTransformer::RoseUniqueCallpathTransformer(InputMapping mapping)
		: RosePassImplementation(), mapping(std::move(mapping)), callGraph(nullptr) {}

RoseUniqueCallpathTransformer::~RoseUniqueCallpathTransformer() {}

RoseUniqueCallpathTransformer::NodeSet RoseUniqueCallpathTransformer::retrieveInputNodes(PassType passType) {
	auto channelIter = mapping.find(passType);
	if (channelIter != mapping.end()) {
		return retrieveInputNodes(channelIter->second);
	} else {
		return NodeSet();
	}
}

RoseUniqueCallpathTransformer::NodeSet RoseUniqueCallpathTransformer::retrieveInputNodes(int channel) {
	auto graphNodes = callGraph->getNodeSetByCS(getInput(channel));
	NodeSet graphNodesUnordered;
	graphNodesUnordered.reserve(graphNodes.size());
	graphNodesUnordered.insert(graphNodes.begin(), graphNodes.end());

	return graphNodesUnordered;
}

InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode *RoseUniqueCallpathTransformer::getMainFunctionNode() {
	if (SgFunctionDeclaration *mainDecl = SageInterface::findMain(SageInterface::getProject())) {
		if (SgFunctionDefinition *mainDef = mainDecl->get_definition()) {
			auto mainConstruct = InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(mainDef);
			ConstructSet mainCS;
			InstRO::InfrastructureInterface::ConstructSetCompilerInterface(&mainCS).put(mainConstruct);

			if (ExtendedCallGraphNode *node = callGraph->getNodeWithExactConstructSet(mainCS)) {
				return node;
			} else {
				throw std::string("Failed to get the call graph node of the main function");
			}
		} else {
			throw std::string("The main function does not have an available definition");
		}
	} else {
		throw std::string("Failed to find the main function");
	}
}

RoseUniqueCallpathTransformer::NodeSet RoseUniqueCallpathTransformer::getSuccessorFunctions(
		ExtendedCallGraphNode *node) {
	NodeSet result;
	auto successors = callGraph->getSuccessors(node);
	result.reserve(successors.size());
	std::deque<ExtendedCallGraphNode *> work(successors.begin(), successors.end());

	while (!work.empty()) {
		ExtendedCallGraphNode *succ = work.back();
		work.pop_back();

		if (succ->getNodeType() == Tooling::ExtendedCallGraph::FUNCTION) {
			result.insert(succ);
		} else {
			successors = callGraph->getSuccessors(succ);
			work.insert(work.end(), successors.begin(), successors.end());
		}
	}

	return result;
}

RoseUniqueCallpathTransformer::NodeSet RoseUniqueCallpathTransformer::getPredecessorFunctions(
		InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode *node) {
	NodeSet result;
	auto predecessors = callGraph->getPredecessors(node);
	result.reserve(predecessors.size());
	std::deque<ExtendedCallGraphNode *> work(predecessors.begin(), predecessors.end());

	while (!work.empty()) {
		ExtendedCallGraphNode *pred = work.back();
		work.pop_back();

		if (pred->getNodeType() == Tooling::ExtendedCallGraph::FUNCTION) {
			result.insert(pred);
		} else {
			predecessors = callGraph->getPredecessors(pred);
			work.insert(work.end(), predecessors.begin(), predecessors.end());
		}
	}

	return result;
}

SgFunctionDeclaration *RoseUniqueCallpathTransformer::getFunDeclFromNode(ExtendedCallGraphNode *node) {
	ConstructSet cs = node->getAssociatedConstructSet();
	InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci(&cs);
	for (auto construct : csci) {
		auto rc = std::dynamic_pointer_cast<InstRO::Rose::Core::RoseConstruct>(construct);
		if (SgFunctionDefinition *def = isSgFunctionDefinition(rc->getNode())) {
			return def->get_declaration();
		} else if (SgFunctionDeclaration *decl = isSgFunctionDeclaration(rc->getNode())) {
			return decl;
		} else {
			logIt(WARN) << "construct is no function: " << SageInterface::get_name(rc->getNode()) << std::endl;
		}
	}

	throw std::string("Failed to extract function declaration from call graph node");
}

void RoseUniqueCallpathTransformer::execute() {
	// get the call graph
	callGraph = getInstrumentorInstance()->getAnalysisManager()->getECG();

	// use the main function as default root if none have been specified manually
	if (mapping.find(PassType::RootPT) == mapping.end()) {
		rootNodes.clear();
		rootNodes.insert(getMainFunctionNode());
	} else {
		rootNodes = retrieveInputNodes(PassType::RootPT);
	}

	// convert marked functions to call graph nodes
	NodeSet markedNodes = retrieveInputNodes(0);

	// initialize active nodes
	activeNodes = retrieveInputNodes(PassType::ActivePT);

	// find candidates for duplication and nodes which are targeted by a backwards directed edge
	NodeDepthMap candidates;
	NodeSet cycleNodes;
	findCandidates(markedNodes, candidates, cycleNodes);

	// use the found candidates to initialize a priority queue which sorts its entries with ascending depth:
	// nodes with a smaller depth will be processed before those with a larger depth
	std::priority_queue<NodeDepthEntry, std::vector<NodeDepthEntry>, NodeDepthComparator> work(candidates.begin(),
																																														 candidates.end());
	NodeSet visited;
	NodeFunctionDeclarationMap duplicatedNodes;

	while (!work.empty()) {
		NodeDepthEntry entry = work.top();	// entry = (node, depth)
		work.pop();

		// get the associated function
		SgFunctionDeclaration *funDecl = getFunDeclFromNode(entry.first);

		// a node may only be visited once or multiple definitions will be created
		if (visited.find(entry.first) != visited.end()) {
			logIt(DEBUG) << SageInterface::get_name(funDecl) << " has already been visited" << std::endl;
			continue;
		}
		visited.insert(entry.first);

		// safety check, in case a node which starts a cycle somehow manages to sneak in
		if (cycleNodes.find(entry.first) != cycleNodes.end()) {
			logIt(DEBUG) << "Ignore loop node" << std::endl;
			continue;
		}

		// debug output...
		logIt(DEBUG) << "Visiting " << SageInterface::get_name(funDecl) << " with depth " << entry.second << std::endl;

		// only active nodes may be transformed (if enabled)
		if (!activeNodes.empty() && activeNodes.find(entry.first) == activeNodes.end()) {
			logIt(WARN) << "Cannot create unique call path because function " << SageInterface::get_name(funDecl)
									<< " is not considered 'active'" << std::endl;
			continue;
		}

		// check whether we are dealing with a real function that has a definition
		funDecl = isSgFunctionDeclaration(funDecl->get_definingDeclaration());
		if (!funDecl || !funDecl->get_definition()) {
			funDecl = getFunDeclFromNode(entry.first);
			logIt(WARN) << "Cannot duplicate function " << SageInterface::get_name(funDecl)
									<< " because no definition is available" << std::endl;
			continue;
		}

		// duplicate function
		logIt(DEBUG) << "Duplicating " << SageInterface::get_name(funDecl) << std::endl;
		duplicate(entry.first, duplicatedNodes);

		// mark successors (functions called by the function which has just been duplicated) for duplication,
		for (ExtendedCallGraphNode *succ : getSuccessorFunctions(entry.first)) {
			work.push(std::make_pair(succ, entry.second + 1));
		}
	}

	callGraph = nullptr;
}

void RoseUniqueCallpathTransformer::findCandidates(const NodeSet &markedNodes, NodeDepthMap &candidates,
																									 NodeSet &cycleNodes) {
	NodeList localPath;

	// start a depth first search starting at each root and update candidates if a marked node appears
	// at the end of the current path through the call graph
	for (ExtendedCallGraphNode *rootNode : rootNodes) {
		// initialize data structures
		localPath.clear();

		// start traversal at the root node at depth 0
		std::deque<ExtendedCallGraphNode *> workStack;
		std::deque<int> depthStack;
		workStack.push_back(rootNode);
		depthStack.push_back(0);

		while (!workStack.empty()) {
			ExtendedCallGraphNode *node = workStack.back();
			workStack.pop_back();
			auto depth = depthStack.back();
			depthStack.pop_back();

			// check for cycle: we cannot simply check whether we have already visited this node
			// because a node may be visited more than once
			if (cycleNodes.find(node) != cycleNodes.end()) {
				// skip cycles which have already been found
				// alternatively, one could just remove the node from the workStack once a cycle has been found for the first
				// time
				continue;
			}
			// a node is considered to start a cycle if it is contained in the current working stack
			if (std::find(workStack.begin(), workStack.end(), node) != workStack.end()) {
				logIt(WARN) << "Found cycle at " << node->getAssociatedConstructSet().toString() << std::endl;
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
			auto successors = getSuccessorFunctions(node);
			for (ExtendedCallGraphNode *succ : successors) {
				// check for recursion because it is not detected by the cycle check above
				if (succ == node) {
					logIt(WARN) << "Found recursion at " << node->getAssociatedConstructSet().toString() << std::endl;
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
	for (ExtendedCallGraphNode *cn : cycleNodes) {
		candidates.erase(cn);
	}
}

void RoseUniqueCallpathTransformer::updateCandidates(NodeDepthMap &candidates, const NodeList &path) {
	logIt(DEBUG) << "Found path: ";
	for (NodeDepthMap::mapped_type i = 0; i < path.size(); ++i) {
		// check whether node is a candidate (has more than one predecessor)
		if (getPredecessorFunctions(path[i]).size() > 1) {
			auto candidateIter = candidates.find(path[i]);
			if (candidateIter != candidates.end()) {
				// update max call depth
				candidateIter->second = std::max(i, candidateIter->second);
			} else {
				// initialize max call depth
				candidates[path[i]] = i;
			}
		}

		logIt(DEBUG) << SageInterface::get_name(getFunDeclFromNode(path[i])) << " ";
	}
	logIt(DEBUG) << std::endl;
}

std::string RoseUniqueCallpathTransformer::generateCloneName(SgFunctionDeclaration *caller,
																														 SgFunctionDeclaration *callee) {
	return callee->get_name().getString() + "_ucp_" + caller->get_name().getString();
}

void RoseUniqueCallpathTransformer::addNodeToCS(InstRO::InfrastructureInterface::ConstructSetCompilerInterface &csci,
																								SgNode *node) {
	csci.put(InstRO::Rose::Core::RoseConstructProvider::getInstance().getConstruct(node));
}

void RoseUniqueCallpathTransformer::duplicate(ExtendedCallGraphNode *node,
																							NodeFunctionDeclarationMap &duplicatedNodes) {
	// retrieve the defining function declaration for the node
	SgFunctionDeclaration *funDecl = isSgFunctionDeclaration(getFunDeclFromNode(node)->get_definingDeclaration());

	// if a parent has already been duplicated, the clones must be used instead as they are not picked up be the 'old'
	// call graph
	auto parents = getPredecessorFunctions(node);
	std::vector<SgFunctionDeclaration *> callingFunctions;
	callingFunctions.reserve(parents.size());

	for (ExtendedCallGraphNode *parentNode : parents) {
		auto range = duplicatedNodes.equal_range(parentNode);

		if (range.first == duplicatedNodes.end()) {
			// parent has not been duplicated
			// the function definition of the parent should always be available - otherwise it would have been difficult to
			// construct the call graph...
			SgFunctionDeclaration *definingParentDecl =
					isSgFunctionDeclaration(getFunDeclFromNode(parentNode)->get_definingDeclaration());
			callingFunctions.push_back(definingParentDecl);
		} else {
			// parent has been duplicated, add all created duplicates
			std::for_each(range.first, range.second, [&](const NodeFunctionDeclarationMap::value_type &entry) {
				callingFunctions.push_back(entry.second);
			});
		}
	}

	// create a duplicate of the function for each function calling it
	for (SgFunctionDeclaration *callingFunDecl : callingFunctions) {
		InstRO::Rose::Utility::FunctionRenamer renamingProvider(funDecl);
		// look for function calls to the function to be duplicated inside the definition of the calling parent function
		renamingProvider.findFunctionCalls(std::vector<SgNode *>{callingFunDecl});

		// clone the function and insert the clone after the original
		SgFunctionDeclaration *clonedFunction =
				cloneFunction(funDecl, generateCloneName(callingFunDecl, funDecl), funDecl->get_scope());
		SageInterface::insertStatementAfter(funDecl, clonedFunction);
		// rename the previously found calls to the original so that they call the clone
		renamingProvider.redirectFunctionCalls(clonedFunction);

		// remember the duplicate for successors of the node which might have to be duplicated
		duplicatedNodes.insert(std::make_pair(node, clonedFunction));

		InstRO::InfrastructureInterface::ConstructSetCompilerInterface output(&outputSet);
		InstRO::InfrastructureInterface::ConstructSetCompilerInterface collisions(&collisionSet);
		// add the duplicate to the output of the transformer and its files scope to the collision set
		addNodeToCS(output, clonedFunction->get_definition());
		addNodeToCS(collisions, SageInterface::getEnclosingFileNode(clonedFunction));
		// add the redirected function calls to the output and the enclosing function to the collision set
		for (SgFunctionCallExp *funCall : renamingProvider.getFoundFunctionCalls()) {
			addNodeToCS(output, funCall);
			addNodeToCS(collisions, SageInterface::getEnclosingFunctionDefinition(funCall));
		}
	}
}

SgFunctionDeclaration *RoseUniqueCallpathTransformer::cloneFunction(SgFunctionDeclaration *originalFunction,
																																		const std::string &cloneName,
																																		SgScopeStatement *scope) {
	// build the list of function parameters
	const SgInitializedNamePtrList &originalArgs = originalFunction->get_args();
	SgFunctionParameterList *parameterList = SageBuilder::buildFunctionParameterList();
	for (const SgInitializedNamePtr arg : originalArgs) {
		SageInterface::appendArg(parameterList, SageBuilder::buildInitializedName(arg->get_name(), arg->get_type()));
	}

	SgExprListExp *decorators = SageInterface::deepCopy(originalFunction->get_decoratorList());

	// check if there is a non defining function declaration available for building the defining one
	SgFunctionDeclaration *nondefiningClonedFunction;
	SgFunctionSymbol *funcSymbol =
			SageInterface::lookupFunctionSymbolInParentScopes(cloneName, originalFunction->get_type(), scope);
	if (funcSymbol) {
		nondefiningClonedFunction = funcSymbol->get_declaration();
	} else {
		nondefiningClonedFunction = SageBuilder::buildNondefiningFunctionDeclaration(
				cloneName, originalFunction->get_orig_return_type(), parameterList, scope, decorators);
		nondefiningClonedFunction->set_exceptionSpecification(originalFunction->get_exceptionSpecification());
	}

	// build a defining function declaration for the clone using a undefined one (ROSE...)
	SgFunctionDeclaration *clonedFunction = SageBuilder::buildDefiningFunctionDeclaration(
			cloneName, originalFunction->get_orig_return_type(), parameterList, scope, decorators, false,
			isSgFunctionDeclaration(nondefiningClonedFunction->get_firstNondefiningDeclaration()));

	// copy the exception specification
	clonedFunction->set_exceptionSpecification(originalFunction->get_exceptionSpecification());
	SgBasicBlock *clonedBody = SageInterface::deepCopy(originalFunction->get_definition()->get_body());
	SageInterface::replaceStatement(clonedFunction->get_definition()->get_body(), clonedBody);

	return clonedFunction;
}
