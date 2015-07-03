#ifndef INSTRO_LLVM_BLACKWHITELISTSELECTOR
#define INSTRO_LLVM_BLACKWHITELISTSELECTOR

#include "clang/AST/RecursiveASTVisitor.h"

#include "instro/clang/core/ConstructSet.h"
#include "instro/utility/BWLFileReader.h"
#include "instro/clang/core/ClangAdapterPass.h"

namespace InstRO {
namespace Clang {
/*
 * Implements the black and white list selection process.
 * XXX Double check semantics with Christian/Roman
 */
class BlackWhitelistSelector
		: public InstRO::Clang::Core::ClangPassImplementation {
 public:
	BlackWhitelistSelector(std::vector<std::string> blacklist,
												 std::vector<std::string> whitelist);
	bool VisitFunctionDecl(clang::FunctionDecl *decl);

	void readFilterFile(std::string filename);

	void init() override;
	void execute() override;
	void finalize() override;
	void releaseOutput() override;
	InstRO::Clang::ClangConstructSet *getOutput() override;

	bool isOnList(std::string functionName, std::vector<std::string> &list);

	void addBlacklistEntry(std::string functionName);
	void addWhitelistEntry(std::string functionName);

 private:
	std::vector<std::string> whitelist;
	std::vector<std::string> blacklist;
	InstRO::Clang::ClangConstructSet cs;
};
}	// Clang
}	// InstRO
#endif
