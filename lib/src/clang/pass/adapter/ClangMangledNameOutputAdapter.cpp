#include "instro/clang/pass/adapter/ClangMangledNameOutputAdapter.h"

#include "instro/clang/core/ClangConstruct.h"
#include "instro/utility/Logger.h"

#include "clang/AST/Mangle.h"	// clang::MangleContext

using namespace InstRO::Clang::Adapter;

ClangMangledNameOutputAdapter::ClangMangledNameOutputAdapter()
		: InstRO::Clang::ClangPassImplBase<ClangMangledNameOutputAdapter>(
					new InstRO::Clang::NonVisitingPassExecuter<ClangMangledNameOutputAdapter>()),
			outfileName("instro-temp-file") {}

bool ClangMangledNameOutputAdapter::VisitFunctionDecl(clang::FunctionDecl* fDecl) { return true; }

void ClangMangledNameOutputAdapter::exec() {
	if (context == nullptr) {
		logIt(ERROR) << "ASTContext was null" << std::endl;
		exit(-1);
	}
	std::ofstream outStream;
	outStream.open(outfileName.c_str());

	if (outStream) {
		auto inputSet = getInput(0);
		if (inputSet == nullptr) {
			logIt(ERROR) << "inputSet was nullptr" << std::endl;
			exit(-1);
		}

		print(outStream, inputSet, context);
	}

	outStream.close();
}

void ClangMangledNameOutputAdapter::print(std::ostream& outStream, const InstRO::Core::ConstructSet* cs,
																					clang::ASTContext* astContext) {
	outStream << "Printing ConstructSet " << cs << "\n";
	InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface rcsci(cs);
	for (auto& c : rcsci) {
		auto construct = std::dynamic_pointer_cast<InstRO::Clang::Core::ClangConstruct>(c);
		if (clang::Decl* decl = construct->getAsDecl()) {
			clang::NamedDecl* nDecl = llvm::dyn_cast<clang::NamedDecl>(decl);
			if (nDecl) {
				auto mc = astContext->createMangleContext();
				std::string str;
				llvm::raw_string_ostream s(str);
				mc->mangleName(nDecl, s);
				outStream << nDecl->getNameAsString() << " :: " << s.str() << "\n";
			}
		} else if (clang::Stmt* stmt = construct->getAsStmt()) {
			std::string str;
			llvm::raw_string_ostream s(str);
			stmt->printPretty(s, nullptr, astContext->getPrintingPolicy());
			outStream << s.str() << "\n";
		}
	}
	outStream << std::endl;
}
