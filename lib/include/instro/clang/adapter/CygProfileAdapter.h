#ifndef INSTRO_CLANG_CYGPROFILEADAPTER_H
#define INSTRO_CLANG_CYGPROFILEADAPTER_H

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Tooling/Core/Replacement.h"

#include "instro/clang/core/ConstructSet.h"
#include "instro/clang/core/Pass.h"

namespace InstRO {
namespace Clang {

class CygProfileAdapter : public ::InstRO::Pass,
													public clang::RecursiveASTVisitor<CygProfileAdapter> {
 public:
	CygProfileAdapter(::InstRO::Pass *selector, clang::SourceManager *sm);

	void run();

	void adapt(::InstRO::Clang::ClangConstruct c);

	void transform(clang::SourceManager *sm, clang::Decl *decl);

 private:
	Pass *decidingSelector;
	ClangConstructSet cs;
	clang::SourceManager *sm;
	clang::tooling::Replacements *replacements;
};

}	// Clang
}	// INstrO

#endif
