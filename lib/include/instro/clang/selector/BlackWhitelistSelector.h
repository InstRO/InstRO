#ifndef INSTRO_LLVM_BLACKWHITELISTSELECTOR
#define INSTRO_LLVM_BLACKWHITELISTSELECTOR

#include "clang/AST/RecursiveASTVisitor.h"

#include "instro/support/BWLFileReader.h"

namespace InstRO {
namespace Clang {
/*
 * Implements the black and white list selection process.
 * XXX Double check semantics with Christian/Roman
 */
class BlackWhitelistSelector
		: public InstRO::Clang::Core::ClangPassImplementation {
 public:
	bool VisitFunctionDecl(clang::FunctionDecl *decl);

	void readFilterFile(std::string filename);

	bool isOnList(std::string functionName, std::vector<std::string> &list);

	void addBlacklistEntry(std::string functionName);
	void addWhitelistEntry(std::string functionName);

 private:
	std::vector<std::string> whitelist;
	std::vector<std::string> blacklist;
};
}	// Clang
}	// InstRO
#endif
