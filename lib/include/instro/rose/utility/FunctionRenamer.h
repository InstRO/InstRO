#ifndef INSTRO_ROSE_UTILITY_FUNCTIONRENAMER_H
#define INSTRO_ROSE_UTILITY_FUNCTIONRENAMER_H

#include "rose.h"

namespace InstRO {
namespace Rose {

/// \brief A utility class for finding function calls of a specific function and renaming them or the definition of the function.
/// \author Simon Reuß
class FunctionRenamer {

public:
    typedef std::vector<SgFunctionCallExp*> FunCallContainer;

    FunctionRenamer(SgFunctionDeclaration *targetFunction);

    const FunCallContainer& findFunctionCalls(const std::vector<SgNode*> &startingPoints);
    /// Renames the specified defining function declaration providing a forward declaration for the original declaration
    /// and a symbol table entry for the renamed one.
    void renameFunctionDefinition(const std::string &newName);
    /// Redirects all found function call expressions to target the specified function declaration.
    void redirectFunctionCalls(SgFunctionDeclaration *newTarget);
    /// Redirect the specified function call to target the specified function declaration.
    void redirectFunctionCall(SgFunctionCallExp* functionCall, SgFunctionDeclaration* newTarget);

    const FunCallContainer& getFoundFunctionCalls() const;

private:
    SgFunctionDeclaration *targetFunction;
    FunCallContainer functionCalls;

};

}
}

#endif // INSTRO_ROSE_UTILITY_FUNCTIONRENAMER_H
