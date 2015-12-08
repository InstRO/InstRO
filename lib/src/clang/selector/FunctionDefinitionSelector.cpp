#include "instro/clang/selector/FunctionDefinitionSelector.h"

#include "instro/clang/core/ClangConstruct.h"
#include "instro/utility/Logger.h"

InstRO::Clang::FunctionDefinitionSelector::FunctionDefinitionSelector()
		: ClangPassImplBase<FunctionDefinitionSelector>(
					InstRO::Core::ChannelConfiguration(), new InstRO::Clang::VisitingPassExecuter<FunctionDefinitionSelector>()) {
}

bool InstRO::Clang::FunctionDefinitionSelector::VisitFunctionDecl(clang::FunctionDecl *fDecl) {
	logIt(DEBUG) << "InstRO::Clang::FunctionDefinitionSelector::VisitFunctionDecl" << std::endl;
	if (fDecl->hasBody()) {
		// we want to select
		logIt(DEBUG) << "selecting node" << fDecl << std::endl;
		InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci(&outputSet);
		csci.put(std::make_shared<InstRO::Clang::Core::ClangConstruct>(fDecl));
	}
	return true;
}
