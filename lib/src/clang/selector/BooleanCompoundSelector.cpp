#include "instro/clang/selector/BooleanCompoundSelector.h"

InstRO::Clang::BooleanCompoundSelector::BooleanCompoundSelector(
		InstRO::Pass *inA, InstRO::Pass *inB)
		: inA(inA), inB(inB) {}

void InstRO::Clang::BooleanCompoundSelector::init() {}
void InstRO::Clang::BooleanCompoundSelector::execute() {
	auto setA = getInput(inA);
	auto setB = getInput(inB);

	InstRO::Clang::ClangConstructSet *ccsA =
			reinterpret_cast<InstRO::Clang::ClangConstructSet *>(setA);
	InstRO::Clang::ClangConstructSet *ccsB =
			reinterpret_cast<InstRO::Clang::ClangConstructSet *>(setB);

	// evaluate a boolean predicate
}

void InstRO::Clang::BooleanCompoundSelector::doOr(
		InstRO::Clang::ClangConstructSet &a, InstRO::Clang::ClangConstructSet &b) {
	for (auto c : a.getConstructSet()) {
		cs.put(InstRO::Clang::getAsDecl(c));
	}
	for (auto c : b.getConstructSet()) {
		cs.put(InstRO::Clang::getAsDecl(c));
	}
}

void InstRO::Clang::BooleanCompoundSelector::doAnd(
		InstRO::Clang::ClangConstructSet &a, InstRO::Clang::ClangConstructSet &b) {
	// I am pretty sure this can be improved.
	// the problem is, that ClangConstruct wraps around Decl, Stmt or Type. So I
	// am not sure whether we can use stl algorithms to do the actual operation.
	// The operation is: if a construct is in both sets, put it in the output set.
	for (auto c : a.getConstructSet()) {
		clang::Decl *d = InstRO::Clang::getAsDecl(c);
		for (auto cc : b.getConstructSet()) {
			clang::Decl *dd = InstRO::Clang::getAsDecl(cc);
			if (d == dd) {
				cs.put(d);
			}
		}
	}
}

void InstRO::Clang::BooleanCompoundSelector::finalize() {}
void InstRO::Clang::BooleanCompoundSelector::releaseOutput() {}
InstRO::Clang::ClangConstructSet *
InstRO::Clang::BooleanCompoundSelector::getOutput() {}

