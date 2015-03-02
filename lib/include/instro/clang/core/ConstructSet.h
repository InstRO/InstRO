#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Clang {

/*
 * Helper data structure to have some common base class
 * for Clang Stmt / Decl / Type voodoo
 */
struct ClangConstruct {
	clang::Decl *decl;
	clang::Stmt *stmt;
	clang::Type *type;
};

template <typename T>
T getAs(const ClangConstruct &cc) {
	if (dynamic_cast<T>(cc.decl) != NULL) {
		return cc.decl;
	}
	if (dynamic_cast<T>(cc.stmt) != NULL) {
		return cc.stmt;
	}
	if (dynamic_cast<T>(cc.type) != NULL) {
		return cc.type;
	}
	assert(false && "default reached");
}

class ClangConstructSet : public ::InstRO::Core::ConstructSet {
 public:
	void put(clang::Decl *decl);
	void put(clang::Stmt *stmt);
	std::vector<ClangConstruct> getConstructSet();

 private:
	std::vector<ClangConstruct> constructs;
};

}	// Clang
}	// InstRO
