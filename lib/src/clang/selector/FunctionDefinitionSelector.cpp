#include "instro/clang/selector/FunctionDefinitionSelector.h"

InstRO::Clang::FunctionDefinitionSelector::FunctionDefinitionSelector(){}

bool  InstRO::Clang::FunctionDefinitionSelector::VisitFunctionDecl(clang::FunctionDecl *fDecl){
	if(fDecl->hasBody()){
		// we want to select
		std::cout << "selecting node" << fDecl << std::endl;
		cs.put(fDecl);
	}
	return true;
}

InstRO::Clang::ClangConstructSet *InstRO::Clang::FunctionDefinitionSelector::getOutput(){
	std::cout << "Retrieving function defintion selector output set." << std::endl;
	return &cs;
}

