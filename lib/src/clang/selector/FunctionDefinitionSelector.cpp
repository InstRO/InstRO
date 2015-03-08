#include "instro/clang/selector/FunctionDefinitionSelector.h"

InstRO::Clang::FunctionDefinitionSelector::FunctionDefinitionSelector(){}

bool  InstRO::Clang::FunctionDefinitionSelector::VisitFunctionDecl(clang::FunctionDecl *fDecl){
	if(fDecl->hasBody()){
		// we want to select
		cs.put(fDecl);
	}
	return true;
}
