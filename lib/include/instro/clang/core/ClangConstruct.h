#ifndef INSTRO_CLANG_CORE_CLANGCONSTRUCT_H
#define INSTRO_CLANG_CORE_CLANGCONSTRUCT_H

#include "instro/core/ConstructSet.h"

#include "clang/AST/Decl.h"
#include "clang/AST/Stmt.h"

namespace InstRO {
namespace Clang {

namespace Support {
class InstROASTConsumer;	// forward declaration
}

namespace Core {

/**
 * @brief The kind of node a ClangConstruct represents.
 */
enum class ConstructKind {
	CK_Declaration = 1,
	CK_Stmt = 2,
};

/**
 * @brief A Construct representing AST nodes of the Clang compiler infrastructure.
 * A ClangConstruct represents either a declaration or a statement.
 * Use getAsDecl() or getAsStmt() to retrieve the underlying Clang node or to check the type of node represented by this
 *construct.
 *
 * @todo
 * - Phantom constructs for empty for-loops, e.g. \c for(;;):
 * May be solved by subclassing ClangConstruct to provide *PhantomClangConstruct* instances for the child
 *statements/expression of the for-loop. The ECG and CFG need constructs in order to build nodes representing missing
 *statements/expressions. Therefore, fixing this is only necessary once these graphs are being implemented for Clang.
 * - Configurability for constructor behavior.
 *
 * ### Differences to the ROSE implementation
 * - A ClangConstruct can be both a statement and an expression in terms of ConstructTraitType at the same time as there
 *is no dedicated expression-statement in Clang. Determining whether a clang::Expr is a statement in terms of
 *ConstructTraitType is not trivial. In essence, the implementation checks whether the parent of a clang::Expr is also a
 *clang::Expr. The ClangTestAdapter contains additional logic to simulate distinct constructs for statements and
 *expressions.
 * - There are no dedicated assign initializer nodes in Clang. Instead, a variable declaration contains a pointer to the
 *expression used to initialize the variable. Again, the ClangTestAdapter attempts to simulate their existence.
 * - Name of nested functions: Clang includes the name and parameter types of the enclosing function
 * - Template instantiations: The Clang AST includes the original tree with the template parameters and copies for each
 *instantiated type. In contrast to ROSE, these copies retain the source location of the original.
 */
class ClangConstruct : public InstRO::Core::Construct {
	friend class Support::InstROASTConsumer;	// calls setASTContext

 public:
	ClangConstruct(clang::Decl* decl);
	ClangConstruct(clang::Stmt* stmt);

	size_t getID() const override;
	std::string getIdentifier() const override;

	std::string toString() const override;
	std::string toDotString() const override;

	/**
	 * @brief Retrieves the declaration represented by this construct.
	 * @return \c nullptr if no declaration is associated with this construct
	 */
	clang::Decl* getAsDecl() const;
	/**
	 * @brief Retrieves the statement represented by this construct.
	 * @note In the Clang AST each expression is also a statement.
	 * @return \c nullptr if no statement is associated with this construct
	 */
	clang::Stmt* getAsStmt() const;

	static clang::SourceManager& getSourceManager();

 private:
	ConstructKind kind;
	void* construct;

	std::string getFunctionName(clang::FunctionDecl* decl) const;

	static clang::ASTContext* astContext;

	static clang::ASTContext& getASTContext();
	static void setASTContext(clang::ASTContext& context);
};
}	// namespace Core
}	// namespace Clang
}	// namespace InstRO

#endif	// INSTRO_CLANG_CORE_CLANGCONSTRUCT_H
