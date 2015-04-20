#ifndef INSTRO_CLANG_CYGPROFILEADAPTER_H
#define INSTRO_CLANG_CYGPROFILEADAPTER_H

#include <cassert>

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Tooling/Core/Replacement.h"

#include "instro/clang/core/ConstructSet.h"
#include "instro/clang/core/Pass.h"
#include "instro/clang/core/ClangAdapterPass.h"

namespace InstRO {
namespace Clang {

class CygProfileAdapter : public InstRO::Clang::Core::ClangPassImplementation {
 public:
	CygProfileAdapter(InstRO::Pass *selector, clang::tooling::Replacements &replacements, clang::SourceManager *sm);

	bool VisitFunctionDecl(clang::FunctionDecl *decl) override;

	void init();

	void execute();

	void finalize();

	void releaseOutput();

	InstRO::Clang::ClangConstructSet *getOutput();

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

 private:
	Pass *decidingSelector;
	ClangConstructSet cs;
	clang::SourceManager *sm;
	clang::tooling::Replacements &replacements;
	int labelCount;
	const std::string cygProfFuncPtrName;
};

}	// Clang
}	// INstrO

#endif
