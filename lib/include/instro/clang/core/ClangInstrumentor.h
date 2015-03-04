#ifndef INSTRO_CLANG_CLANGINSTRUMENTOR_H
#define INSTRO_CLANG_CLANGINSTRUMENTOR_H

#include "clang/Tooling/Tooling.h"
#include "clang/Tooling/Refactoring.h"

#include "instro/clang/core/PassFactory.h"

namespace InstRO {
namespace Clang {

/*
 * In order to be runnable with Clang's refactoring tool this class has the
 * getClangConsumer() method, which will create the necessary instance and
 * return
 * to the caller
 */
class ClangInstrumentor {
 public:
 	ClangInstrumentor(::InstRO::Core::PassManagement::PassManager *manager) : fac(manager){}
	clang::tooling::ToolAction* getClangAction();
	::InstRO::Clang::PassFactory * getFactory();

private:
	::InstRO::Clang::PassFactory fac;

};
}	// Clang
}	// INstRO
#endif
