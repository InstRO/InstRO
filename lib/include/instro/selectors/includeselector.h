#include "../common.h"

/**
 * FIXME 2013-10-07 JP: This whole class needs some attention! Get into the Rose header handling as discussed with DQ.
 */


/**
 * \brief Adds SgFile-Nodes for included files.
 *
 * \todo 2013-10-08 JP: Work on this Selector!! It is not stable.
 *
 * This selector does not select any nodes, it merely remodels the ast.
 * Since only the files passed on command line are represented as SgFile-Nodes in the AST, this selector
 * adds these Nodes for all files included with quotation marks, so that the instrumented versions may be unparsed.
 * Should be added in front of all other selectors, since it severely modifies the AST and adds new nodes.
 * \ingroup Selector
 */
class IncludeSelector : public Selector{
public:
    IncludeSelector(SgProject* project);
    void traverseAST(SgNode* start);            /**< \brief Remodels the AST. \param start Is discarded and the project given at construction is used */
//    SelectionASTMarker * getSelectionMarker(SgNode* node);
private:
    SgProject* project;

    void select(SgNode* node);  /**< \brief Tells the adapter builder to instrument this node */
};

