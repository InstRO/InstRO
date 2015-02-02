#ifndef GEIMER_ADAPTER_H
#define GEIMER_ADAPTER_H

#include "../common.h"
#include "../utility/instrobuilder.h"

namespace InstRO{
/**
 * \attention This selector example is functional but has not been tested excessively!
 *
 * Backend which is able to build the instrumentation based on the Geimer specification
 * by inserting user defined code snippets at the specified locations.
 *
 * Works in with the GeimerSelector. As user you will need a configuration file with a name corresponding
 * to the name of your source file. So as you want to instrument foo.cpp you will need foo.cfg - for bar.cpp a bar.cfg, respectively.
 *
 * \ingroup Adapterbuilder
 * \author Jan-Patrick Lehr
 */
class GeimerAdapterBuilder : public AdapterBuilder{
public:
    /** The selector passed should be a GeimerSelector. If not this will result in undefined behaviour. (May include Seg faults.) */
    GeimerAdapterBuilder(Selector* selectorToRespect);
    /* CI FAST BUGFIX. crashes */
    /* JPL: Now should not crash anylonger */
    /*GeimerAdapterBuilder(SgProject* specAst, Selector* selectorToRespect);*/
    /** Traverses the nodes, marked by the GeimerSelector and building the instrumentation code. */
    void buildForNode(SgNode* n, Selector* selectorToRespect);
    void buildForNode(SgNode* n);
private:
    SgProject* specProj;
    Selector* selectorToRespect;

    void populateVariableDeclarations(SgNode* n);
    void populateEntryConstructs(SgNode* n);
    void populateExitConstructs(SgNode* n);
    void populateAbortConstructs(SgNode* n);
    void populateInitConstructs(SgNode* n);

    void populateMainInitConstructs(SgNode* n);
    void populateIncludeConstructs(SgNode* n);

    void transformReturn(SgReturnStmt* returnStatement);
    void extractConstructsFromMarker(std::tr1::shared_ptr<GeimerSelectionASTMarker> marker);
    void insertIntoDataContainer(SgNode* tNode, std::string pType);
    std::vector<SgNode*> entryNodes;
    std::vector<SgNode*> exitNodes;
    std::vector<SgNode*> declNodes;
    std::vector<SgNode*> abortNodes;
    std::vector<SgNode*> initNodes;
    std::vector<SgNode*> mainInitNodes;
    std::vector<SgNode*> includeNodes;
    bool includeHasBeenProcessed;
};
}
#endif
