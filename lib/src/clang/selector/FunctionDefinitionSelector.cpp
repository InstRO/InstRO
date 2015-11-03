#include "instro/clang/selector/FunctionDefinitionSelector.h"
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
		cs.put(fDecl);
	}
	return true;
}

InstRO::Clang::ClangConstructSet *InstRO::Clang::FunctionDefinitionSelector::getOutput() {
	logIt(DEBUG) << "Retrieving function defintion selector output set." << std::endl;
	return &cs;
}
