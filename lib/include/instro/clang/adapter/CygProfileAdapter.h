#ifndef INSTRO_CLANG_CYGPROFILEADAPTER_H
#define INSTRO_CLANG_CYGPROFILEADAPTER_H

#include <cassert>

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Tooling/Core/Replacement.h"

#include "instro/clang/core/ConstructSet.h"
#include "instro/clang/core/Pass.h"
#include "instro/clang/core/ClangAdapterPass.h"

namespace InstRO {
namespace Clang {

#if 0
class CygProfileAdapter : public InstRO::PassImplementation,
													public InstRO::Clang::Core::ClangAdapterPass,
													public clang::RecursiveASTVisitor<CygProfileAdapter> {
#else
class CygProfileAdapter : public InstRO::Clang::Core::ClangPassImplementation {
#endif

public:
CygProfileAdapter(InstRO::Pass *selector, clang::SourceManager *sm);

void init();

void execute();

void finalize();

void releaseOutput();

InstRO::Clang::ClangConstructSet *getOutput();

void adapt(InstRO::Clang::ClangConstruct c);

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
