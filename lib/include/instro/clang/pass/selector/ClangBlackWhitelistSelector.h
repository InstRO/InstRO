#ifndef INSTRO_CLANG_PASS_SELECTOR_CLANGBLACKWHITELISTSELECTOR
#define INSTRO_CLANG_PASS_SELECTOR_CLANGBLACKWHITELISTSELECTOR

#include "clang/AST/RecursiveASTVisitor.h"

#include "instro/clang/core/ClangPassImplBase.h"
#include "instro/utility/BWLFileReader.h"

namespace InstRO {
namespace Clang {
namespace Selector {

/*
 * Implements the black and white list selection process.
 * FIXME Double check semantics with Christian/Roman
 */
class ClangBlackWhitelistSelector : public InstRO::Clang::ClangPassImplBase<ClangBlackWhitelistSelector> {
 public:
	ClangBlackWhitelistSelector(std::vector<std::string> blacklist, std::vector<std::string> whitelist);
	bool VisitFunctionDecl(clang::FunctionDecl* decl);

	void readFilterFile(std::string filename);

	bool isOnList(std::string functionName, std::vector<std::string>& list);

	void addBlacklistEntry(std::string functionName);
	void addWhitelistEntry(std::string functionName);

 private:
	std::vector<std::string> blacklist;
	std::vector<std::string> whitelist;
};
}	// namespace Selector
}	// namespace Clang
}	// namespace InstRO
#endif
