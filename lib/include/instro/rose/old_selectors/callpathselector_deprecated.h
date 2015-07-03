#ifndef INSTRUMENTOR_CALLPATH_H__
#define INSTRUMENTOR_CALLPATH_H__

#include "../common.h"

#include <string>


namespace InstRO{
/**
 * Selects functions based on how deep they are in the callpath
 *
 * \todo 2013-10-08 JP: It seems that this class is no more in use, since it is replaced by the callpathselector.h, am I right?
 * \ingroup Selector
 */
class CallpathSelector : public OutOfOrderSelector{
public:
    /* Constructor - builds a list with the distance to the main node for every reachable
   Last parameter is optional, since the call graph does not have to be output */
    CallpathSelector(SgProject* project, int maxDepth);
    CallpathSelector(SgProject* project, int maxDepth, std::string output);

    /* Visiting function - gets called for every node in the AST and decides whether or not to instrument it */
    void 	visit(SgNode* node);

private:
    /* Member declarations */
    /** \brief Struct for the actual entries into the list */
    typedef struct _entry{
        std::string functionName;	/**< The name of the function */
        int callDepth;				/**< The distance of the function from the main-node in the callgraph */
    } entry;

    /** \brief Struct to remember which children to visit */
    typedef struct _tovisit{
        SgGraphNode* node;	/**< The node the specific struct is concerned with */
        bool visited;		/**< Remember if the node is already visited */
    } tovisit;

    /* Member variables */
    std::list<entry> callDepthList; /**< The actual list. Not visible to the user, so we can provide a simple interface for it */
    int maxDepth; /**< The maximal depth up to which nodes should be instrumented */

    /* Handles the initialization in order to make overloaded constructors usable */
    void init(SgProject* project, int maxDepth, std::string output);

    /* Getter - returns the callDepth for the given functionname */
    int		getDepth(const std::string functionName);
    /* Returns true iff the given functionName has an associated depth on the list */
    bool 	isOnList(const std::string functionName);
    /* Deletes the entry with the given functionName from the list */
    void	removeEntry(const std::string functionName);
    /* Checks if functionName already has an entry, writes the new data to it, if it does, creates a new one, if none existed */
    void	updateEntry(const std::string functionName, int depth);

    /* Explicitly adds the given entry, not checking its existence */
    void	addEntry(const std::string functionName, int depth);

    /* Marks all the nodes children as visited and continues traversal in them */
    void	breadthFirstSearch(SgIncidenceDirectedGraph* callGraph, SgGraphNode* currentNode, int currentDepth);

    /* Traverses all Nodes of the given Graph and returns the one node that represents the declaration of the main function */
    SgGraphNode* getMainFunctionNodeFromGraph(SgIncidenceDirectedGraph* callGraph);

    /* Returns all children of the given node */
    std::set<SgGraphNode*> getChildren(SgIncidenceDirectedGraph* callGraph, SgGraphNode* parent);

    /* If the GraphNode represents a FunctionDeclaration, its name is returned */
    std::string getQualifiedName(SgGraphNode* node);

    /* Prints the given callgraph to the given depth in scalasca-compatible format */
    void printCallGraphScalasca(SgIncidenceDirectedGraph* callGraph, unsigned int maxDepth, const std::string outputPath);

    /* Prints the subgraph beginning at root to depth maxDepth, assuming that root is located in depth currentDepth */
    void printSubGraphScalasca(SgIncidenceDirectedGraph* callGraph, SgGraphNode* root, int currentDepth, int maxDepth, std::ofstream* output);

    /* Prints the given node with given depth, taking care of all necessary seperators for scalasca-compatibility. */
    void printNodeScalasca(SgGraphNode* node, int currentDepth, std::ofstream* output);
};
}
#endif // INSTRUMENTOR_CALLPATH_H__
