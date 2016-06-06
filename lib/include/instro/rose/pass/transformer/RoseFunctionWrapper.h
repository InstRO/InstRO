#ifndef INSTRO_ROSE_ADAPTER_ROSE_FUNCTION_WRAPPER_H
#define INSTRO_ROSE_ADAPTER_ROSE_FUNCTION_WRAPPER_H

#include "instro/Pass.h"
#include "instro/rose/core/RosePassImplementation.h"

#include "rose.h"

#include <string>
#include <functional>
#include <unordered_set>

namespace InstRO {

namespace Rose {
namespace Transformer {

/// \brief Wraps functions by creating a new defining function declaration calling the original function
/// or another one derived from the name of the original function.
///
/// This Transformer accepts the following input:
/// \li function call expressions: Attempts to find and wrap the associated function declaration
/// \li function definitions
/// \li function declarations
///
/// As output, the function definition of the created wrapper is selected so that it may be refined by a following
/// adapter.
/// Further output depends on the mode of operation and includes renamed function calls and the renamed original
/// function definition (if applicable).
///
/// The function wrapper supports both functions with and without a definition in the project scope
/// and provides means to specify prefixes for the generated wrappers and renamed function definitions.
/// In order to keep this implementation as generic as possible several input configurations are available:
/// \li A FunctionWrapper::NameTransformer functor which generates the name of the function which is called inside the
/// wrapper
/// if the definition of the function to be wrapped cannot be found. If a definition is found, it is renamed using the
/// transformed name.
/// \li A <tt>definition prefix</tt> which is used to rename the definition of the function to be wrapped.
/// \li A <tt>wrapper prefix</tt> which gets prepended to the name of the wrapper.
/// \li A Selector which specifies the starting points in the AST for the search of calls to the original function.
/// This can be used to prevent function calls from being renamed if the names of the original and the wrapper do not
/// match.
///
/// The transformer attempts to find the translation unit containing the main function where it will place all wrappers.
/// If it does not manage to find it, wrappers are placed in the translation unit of the function declaration.
/// However, in order to circumvent errors at link time due to multiple definitions a postfix generated from the source
/// filename gets appended
/// to the name of the wrapper (and renamed function definition).
///
/// If the name of the wrapper differs from the one of the original function, the FunctionWrapper will search for
/// function calls matching the function at a project scope and update them
/// so that they correctly call the wrapper. Alternatively, only the nodes which have been selected by the optional
/// renaming Selector are traversed.
/// As the definition of the wrapper is appended to the translation unit, an additional declaration must be provided for
/// functions calling the wrapper.
/// Therefore, a forward declaration is inserted before the enclosing function declaration.
///
/// Subclasses may override initMainScope(SgScopeStatement*) in order to perform initialization of the global scope
/// where the main function was found
/// so that all necessary declarations for the wrappers are available.
/// Alternatively, the user is responsible for providing the necessary declarations if the wrappers are inserted into
/// the translation unit of the main function.
/// This can be achieved by inserting the required header file prior to calling the compiler.
///
/// \note The return type and parameter types of the function called inside the wrapper must match the original
/// function!
///
/// \author Simon Reuß
class RoseFunctionWrapper : public RosePassImplementation {
 public:
	/// Alias for the functor type used to generate the name of the function called inside the wrapper.
	typedef std::function<std::string(const std::string&)> NameTransformer;

	/// A NameTransformer which returns its input
	class IdentityNameTransformer {
	 public:
		std::string operator()(const std::string& name);
	};

	/// Constructs a new FunctionWrapper instance which generates wrappers that call a function specified by the
	/// NameTransformer.
	/// \arg input A pass which specifies the functions for which wrappers are created
	/// \arg nameTransformer A functor which determines the name of the function which is called in the wrapper depending
	/// on the name of the original function
	RoseFunctionWrapper(NameTransformer nameTransformer);

	/// Constructs a new FunctionWrapper instance which generates wrappers that call a function specified by the
	/// NameTransformer.
	/// \arg input A pass which specifies the functions for which wrappers are created
	/// \arg useRenamingPass Whether a second input pass that specifies the nodes of the AST that get searched for
	/// function calls that are to be renamed is available
	/// \arg nameTransformer A functor which determines the name of the function which is called in the wrapper depending
	/// on the name of the original function
	/// \arg definitionPrefix The original function definition will be renamed according to this prefix
	/// \arg wrapperPrefix The prefix which gets applied to the name of every generated wrapper
	RoseFunctionWrapper(NameTransformer nameTransformer, const std::string& definitionPrefix,
											const std::string& wrapperPrefix, bool useRenamingPass);

	virtual ~RoseFunctionWrapper();

	virtual void init() override;
	virtual void execute() override;

	std::string getDefinitionPrefix() const;
	void setDefinitionPrefix(const std::string& prefix);

	std::string getWrapperPrefix() const;
	void setWrapperPrefix(const std::string& prefix);

 protected:
	/// \brief Builds the body of the wrapper.
	///
	/// For the default implementation the constructed body consists of an immediate return statement calling the
	/// specified function. If the return type is \c void, only a function call statement is built.
	virtual void buildWrapperBody(SgFunctionDeclaration* fDec, SgFunctionDeclaration* wrapperDec,
																const std::string& functionToCall);

	/// \brief Called once to initialize the global scope of the main function.
	///
	/// Subclasses may implement this method to insert all declarations necessary for the wrappers.
	/// For example, a required header can be inserted. The method is called once prior to the creation of the first
	/// wrapper and only if the global scope of the main function has been found.
	///
	/// \arg scope The global scope of the main function
	virtual void initMainScope(SgScopeStatement* scope);

 private:
	NameTransformer nameTrafo;
	std::string defPrefix;
	std::string wrapPrefix;

	SgScopeStatement* mainScope;

	bool useRenamingPass;

	typedef std::unordered_set<SgNode*> RoseNodeSet;
	typedef std::vector<SgNode*> RoseNodeList;

	RoseNodeSet retrieveNodes(int channel);

	void findMainScope();

	void wrapFunction(SgFunctionDeclaration* node, const RoseNodeList& funCallSearchStartPoints);

	/// Checks whether the input node is a valid input and maps all input nodes to a common node type for the
	/// transformation.
	SgFunctionDeclaration* findInputDeclaration(SgNode* node);
	/// Queries the renaming selector for all selected nodes and caches them.
	void cacheRenamingSP();

	std::string generatePostfix(SgFunctionDeclaration* fDecl);

	/// Clones the function parameters of the specified declaration.
	SgFunctionParameterList* cloneFunctionParameterList(const SgFunctionDeclaration* fDec);

	/// Builds a list of variable references to the specified arguments.
	SgExprListExp* buildFunctionCallArguments(const SgInitializedNamePtrList* argList);
};

/// \brief RoseFunctionWrapper which wraps MPI functions by calling the corresponding PMPI function inside the wrapper.
/// \author Simon Reuß
class RoseMPIFunctionWrapper : public RoseFunctionWrapper {
 public:
	RoseMPIFunctionWrapper();
	RoseMPIFunctionWrapper(const std::string& definitionPrefix, const std::string& wrapperPrefix, bool useRenamingPass);

	/// \brief Functor which transforms MPI function names to their corresponding PMPI name
	/// by prepending a 'P' to the original name.
	class PMPINameTransformer {
	 public:
		std::string operator()(const std::string& name);
	};

 protected:
	/// Inserts the MPI header if the typedef \c MPI_Comm cannot be found in the supplied scope.
	virtual void initMainScope(SgScopeStatement* scope) override;
};
}
}
}
#endif	// INSTRO_ROSE_ADAPTER_ROSE_FUNCTION_WRAPPER_H
