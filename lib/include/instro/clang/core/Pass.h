#ifndef INSTRO_CLANG_PASS_H
#define INSTRO_CLANG_PASS_H

#include "instro/core/Pass.h"

namespace InstRO {
namespace Clang {

class PassImplementation : public InstRO::Core::PassImplementation {
 public:
	void run();
};

}	// clang
}	// instro
#endif
