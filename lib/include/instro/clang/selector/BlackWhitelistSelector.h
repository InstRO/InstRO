#ifndef INSTRO_LLVM_BLACKWHITELISTSELECTOR
#define INSTRO_LLVM_BLACKWHITELISTSELECTOR

#include "clang/AST/RecursiveASTVisitor.h"

#include "instro/utility/BWLFileReader.h"
#include "instro/clang/core/ClangPassImplBase.h"

namespace InstRO {
namespace Clang {
/*
 * Implements the black and white list selection process.
 * FIXME Double check semantics with Christian/Roman
 */
class BlackWhitelistSelector : public InstRO::Clang::ClangPassImplBase<BlackWhitelistSelector> {
 public:
	BlackWhitelistSelector(std::vector<std::string> blacklist, std::vector<std::string> whitelist);
	bool VisitFunctionDecl(clang::FunctionDecl *decl);

	void readFilterFile(std::string filename);

	bool isOnList(std::string functionName, std::vector<std::string> &list);

	void addBlacklistEntry(std::string functionName);
	void addWhitelistEntry(std::string functionName);

 private:
	std::vector<std::string> blacklist;
	std::vector<std::string> whitelist;

};
}	// Clang
}	// InstRO
#endif
