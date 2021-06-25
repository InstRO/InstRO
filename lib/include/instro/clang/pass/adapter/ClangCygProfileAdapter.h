#ifndef INSTRO_CLANG_PASS_ADAPTER_CLANGCYGPROFILEADAPTER_H
#define INSTRO_CLANG_PASS_ADAPTER_CLANGCYGPROFILEADAPTER_H

#include <cassert>
#include <instro.h>

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Tooling/Core/Replacement.h"

#include "instro/clang/core/ClangPassImplBase.h"

namespace InstRO {
namespace Clang {
namespace Adapter {

class ClangCygProfileAdapter : public InstRO::Clang::ClangPassImplBase<ClangCygProfileAdapter> {
 public:
	ClangCygProfileAdapter(ReplacementsMap &replacements, clang::SourceManager *sm);

	bool VisitFunctionDecl(clang::FunctionDecl *decl);

	void dispatch(clang::Decl *c);

	void transform(clang::SourceManager *sm, clang::FunctionDecl *decl);
	void transform(clang::SourceManager *sm, clang::CXXMethodDecl *decl);

 protected:
	std::string generateFunctionEntry(clang::FunctionDecl *d);
	std::string generateFunctionExit(clang::FunctionDecl *d);
	std::string generateMethodEntry(clang::CXXMethodDecl *d);
	std::string generateMethodExit(clang::CXXMethodDecl *d);
	bool isOverloadedFunction(clang::FunctionDecl *decl);
	std::string generateFunctionPointerDecl(std::string name, clang::FunctionDecl *d);
	std::string generateCallTo(std::string fName, std::string newDecl);
	std::string generateCallTo(std::string fName, clang::FunctionDecl *decl);
	void instrumentFunctionBody(clang::CompoundStmt *body, std::string &entryStr, std::string &exitStr);
	void handleEmptyBody(clang::CompoundStmt *body, std::string &entryStr, std::string &exitStr);
	void instrumentReturnStatements(clang::CompoundStmt *body, std::string &entryStr, std::string &exitStr);
	bool retStmtNeedsTransformation(clang::ReturnStmt *st);
	void transformReturnStmt(clang::ReturnStmt *retStmt);

	void insertReplacement(clang::tooling::Replacement rep);

 private:
	clang::SourceManager *sm;
	ReplacementsMap &replacements;
	int labelCount;
	const std::string cygProfFuncPtrName;
};

}	// Adapter
}	// Clang
}	// INstrO

#endif
