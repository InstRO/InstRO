#include "instro/clang/selector/FunctionDefinitionSelector.h"

#include "instro/clang/core/ClangConstruct.h"
#include "instro/utility/Logger.h"

InstRO::Clang::FunctionDefinitionSelector::FunctionDefinitionSelector()
		: ClangPassImplBase<FunctionDefinitionSelector>(
					InstRO::Core::ChannelConfiguration(), new InstRO::Clang::VisitingPassExecuter<FunctionDefinitionSelector>()) {
}

bool InstRO::Clang::FunctionDefinitionSelector::VisitFunctionDecl(clang::FunctionDecl *fDecl) {
	if (fDecl->isThisDeclarationADefinition()) {
		auto construct = std::make_shared<InstRO::Clang::Core::ClangConstruct>(fDecl);
		if (construct->getTraits().is(InstRO::Core::ConstructTraitType::CTFunction)) {
			logIt(DEBUG) << "Selecting " << construct->getIdentifier() << std::endl;
			InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci(&outputSet);
			csci.put(construct);
		}
	}

	return true;
}
