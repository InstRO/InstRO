#ifndef INSTRO_CLANG_BOOLEANCOMPOUNDSELECTOR_H
#define INSTRO_CLANG_BOOLEANCOMPOUNDSELECTOR_H
#include "clang/AST/RecursiveASTVisitor.h"

#include "instro/clang/core/ConstructSet.h"
#include "instro/clang/core/ClangAdapterPass.h"

namespace InstRO {
namespace Clang {
class BooleanCompoundSelector
		: public InstRO::Clang::Core::ClangPassImplementation {
 public:
	BooleanCompoundSelector(InstRO::Pass *inA, InstRO::Pass *inB);

	void init() override;
	void execute() override;
	void finalize() override;
	void releaseOutput() override;
	InstRO::Clang::ClangConstructSet *getOutput() override;

 protected:
	void doOr(InstRO::Clang::ClangConstructSet &a,
						InstRO::Clang::ClangConstructSet &b);
	void doAnd(InstRO::Clang::ClangConstructSet &a,
						 InstRO::Clang::ClangConstructSet &b);

 private:
	InstRO::Pass *inA, *inB;
	InstRO::Clang::ClangConstructSet cs;
};
}
}
#endif
