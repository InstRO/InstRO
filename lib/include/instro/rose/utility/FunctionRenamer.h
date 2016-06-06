#ifndef INSTRO_ROSE_UTILITY_FUNCTIONRENAMER_H
#define INSTRO_ROSE_UTILITY_FUNCTIONRENAMER_H

#include "rose.h"

namespace InstRO {
namespace Rose {
namespace Utility {

/// \brief A utility class for finding function calls of a specific function and renaming them or the definition of the
/// function.
///
/// Used by both the InstRO::Rose::Transformer::RoseUniqueCallpathTransformer and the InstRO::Rose::Transformer::RoseFunctionWrapper.
/// \author Simon Reuß
class FunctionRenamer {
 public:
	typedef std::vector<SgFunctionCallExp *> FunCallContainer;

	FunctionRenamer(SgFunctionDeclaration *targetFunction);

	/// \brief Finds all function call expressions refering to the target function in the sub-ASTs specified by the supplied nodes.
	///
	/// The results are chached within this instance and can be retrieved by #getFoundFunctionCalls().
	const FunCallContainer &findFunctionCalls(const std::vector<SgNode *> &startingPoints);
	/// Renames the specified defining function declaration providing a forward declaration for the original declaration
	/// and a symbol table entry for the renamed one.
	void renameFunctionDefinition(const std::string &newName);
	/// \brief Redirects all found function call expressions to target the specified function declaration.
	///
	/// #findFunctionCalls should have been called before invoking this method. Otherwise, no function calls will be redirected!
	void redirectFunctionCalls(SgFunctionDeclaration *newTarget);
	/// Redirect the specified function call to target the specified function declaration.
	void redirectFunctionCall(SgFunctionCallExp *functionCall, SgFunctionDeclaration *newTarget);
	/// Retrieves the found function call expressions of a previous call to #findFunctionCalls.
	const FunCallContainer &getFoundFunctionCalls() const;

 private:
	SgFunctionDeclaration *targetFunction;
	FunCallContainer functionCalls;
};
}
}
}

#endif	// INSTRO_ROSE_UTILITY_FUNCTIONRENAMER_H
