#include "instro/rose/utility/FunctionRenamer.h"

using namespace InstRO;
using namespace InstRO::Rose;
using namespace InstRO::Rose::Utility;

namespace {

/// Helper class to find calls to a specific function in the AST.
class FunctionCallMatcher
    : public AstSimpleProcessing
{
public:
    FunctionCallMatcher(SgFunctionDeclaration* targetDecl)
        : targetDecl(targetDecl)
        { }

    const FunctionRenamer::FunCallContainer& run(const std::vector<SgNode*> &startingPoints)
    {
        for (SgNode *node : startingPoints) {
            traverse(node, preorder);
        }

        return matchedFunCalls;
    }

protected:
    void visit(SgNode *node) override
    {
        SgFunctionCallExp *funCall = isSgFunctionCallExp(node);

        if (funCall)
        {
            // check whether the called function equals the one we are looking for
            SgFunctionDeclaration *assocDecl = funCall->getAssociatedFunctionDeclaration();
            if (assocDecl)
            {
                if (assocDecl == targetDecl || assocDecl->get_mangled_name() == targetDecl->get_mangled_name())
                {
                    matchedFunCalls.push_back(funCall);
                }
            }
        }
    }
private:
    SgFunctionDeclaration *targetDecl;
    FunctionRenamer::FunCallContainer matchedFunCalls;

};

}


using namespace SageBuilder;
using namespace SageInterface;

FunctionRenamer::FunctionRenamer(SgFunctionDeclaration *targetFunction)
    : targetFunction(targetFunction)
{ }


const FunctionRenamer::FunCallContainer& FunctionRenamer::findFunctionCalls(const std::vector<SgNode*> &startingPoints)
{
    FunctionCallMatcher callMatcher(targetFunction);

    functionCalls = callMatcher.run(startingPoints);
    return functionCalls;
}

const FunctionRenamer::FunCallContainer& FunctionRenamer::getFoundFunctionCalls() const
{
    return functionCalls;
}

void FunctionRenamer::renameFunctionDefinition(const std::string &newName)
{
    SgFunctionDeclaration *definingDecl = targetFunction;
    // Function symbols are associated with the first non defining declaration, defining declarations do not have a symbol themselves.
    // see: https://mailman.nersc.gov/pipermail/rose-public/2010-November/000528.html
    // Therefore, the symbol of the original declaration is kept in tact and just the name of the definition is changed.

    // preceding statements calling the function need a fitting forward declaration when the defining declaration is renamed:
    SgFunctionDeclaration *forwardDecl = buildNondefiningFunctionDeclaration(definingDecl);
    insertStatementBefore(definingDecl, forwardDecl);

    definingDecl->set_name(newName);

    SgScopeStatement *fDeclScope = definingDecl->get_scope();
    // if there is no entry in the symbol table for the new function name, create one
    if (lookupFunctionSymbolInParentScopes(definingDecl->get_name(), definingDecl->get_type(), fDeclScope) == NULL)
    {
        SgFunctionSymbol *originalDefinition = new SgFunctionSymbol(definingDecl);
        fDeclScope->insert_symbol(definingDecl->get_name(), originalDefinition);
    }
}

void FunctionRenamer::redirectFunctionCalls(SgFunctionDeclaration *newTarget)
{
    for (SgFunctionCallExp* callExp : functionCalls)
    {
        redirectFunctionCall(callExp, newTarget);
    }
}

void FunctionRenamer::redirectFunctionCall(SgFunctionCallExp* functionCall, SgFunctionDeclaration* newTarget)
{
    // the wrapper definition is appended to the global scope and may lie in another translation unit:
    // check whether a forward declaration for the new target is necessary
    SgStatement *stmt = getEnclosingStatement(functionCall);
    SgFunctionSymbol *functionSymbol = lookupFunctionSymbolInParentScopes(newTarget->get_name(), newTarget->get_type(), stmt->get_scope());
    bool needForwardDecl = !functionSymbol;
    if (functionSymbol) {
        // if the found declaration is located after the function call, we need a forward declaration after all
        SgFunctionDeclaration *foundFunDecl = functionSymbol->get_declaration();
        // sadly this does not work as intended in ROSE as generated declarations do not seem to have proper placement information
        if (!foundFunDecl->get_file_info()->isSameFile(stmt->get_file_info())) {
            needForwardDecl = true;
        } else {
            needForwardDecl = *foundFunDecl->get_file_info() > *stmt->get_file_info();
        }
    }

    if (needForwardDecl) {
        // build a forward declaration and insert it before the enclosing function declaration
        SgFunctionDeclaration *forwardDecl = buildNondefiningFunctionDeclaration(newTarget, stmt->get_scope());
        insertStatementBefore(getEnclosingFunctionDeclaration(stmt), forwardDecl);
    }

    // modify the target function of the function call
    SgFunctionRefExp *targetRef = buildFunctionRefExp(newTarget);
    replaceExpression(functionCall->get_function(), targetRef, false);
}
