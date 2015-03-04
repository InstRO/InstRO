#ifndef INSTRO_CLANG_CLANGINSTRUMENTOR_H
#define INSTRO_CLANG_CLANGINSTRUMENTOR_H

#include "clang/Tooling/Tooling.h"

#include "instro/clang/core/PassFactory.h"

namespace InstRO {
namespace Clang {

/*
 * In order to be runnable with Clang's refactoring tool this class has the
 * getClangConsumer() method, which will create the necessary instance and
 * return
 * to the caller
 */
class Instrumentor {
 public:
	clang::ASTConsumer* getClangConsumer();
	::InstRO::Clang::PassFactory * getFactory();

private:
	::InstRO::Clang::PassFactory fac;

};
}	// Clang
}	// INstRO
#endif
