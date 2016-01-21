#ifndef INSTRO_CLANG_CORE_CLANGCONSTRUCT_H
#define INSTRO_CLANG_CORE_CLANGCONSTRUCT_H

#include "instro/core/ConstructSet.h"

#include "clang/AST/Decl.h"
#include "clang/AST/Stmt.h"

namespace InstRO {
namespace Clang {
namespace Core {

enum class ConstructKind {
	CK_Declaration = 1,
	CK_Stmt = 2,
};

class ClangConstruct : public InstRO::Core::Construct {
 public:
	ClangConstruct(clang::Decl* decl);
	ClangConstruct(clang::Stmt* stmt);

	size_t getID() const override;
	std::string getIdentifier() const override;

	std::string toString() const override;
	std::string toDotString() const override;

	clang::Decl* getAsDecl() const;
	clang::Stmt* getAsStmt() const;

	static void setASTContext(clang::ASTContext& context);

 private:
	ConstructKind kind;
	void* construct;

	std::string getFunctionName(clang::FunctionDecl* decl) const;

	static clang::ASTContext* astContext;

	static clang::ASTContext& getASTContext();
	static clang::SourceManager& getSourceManager();
};
}
}
}

#endif	// INSTRO_CLANG_CORE_CLANGCONSTRUCT_H
