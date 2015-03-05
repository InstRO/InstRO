#include "instro/clang/support/InstROASTConsumer.h"

::InstRO::Clang::Support::InstROASTConsumer::InstROASTConsumer(
		::InstRO::Core::PassManagement::PassManager *passManager)
		: passManager(passManager) {}

		
void ::InstRO::Clang::Support::InstROASTConsumer::HandleTranslationUnit(
		clang::ASTContext &context) {
	// Is invoked on every TranslationUnit
	// invoke all passes in the correct order per TranslationUnit
}

