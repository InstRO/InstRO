#ifndef INSTRO_CORE_PASSMANAGEMENT_PASSEXECUTER_H
#define INSTRO_CORE_PASSMANAGEMENT_PASSEXECUTER_H

#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace PassManagement {
/*
 * This class is used as a delegate in the PassManager to be unaware
 * of whether different Compiler Backends need different behavior for
 * actually ececuting a PassImplementation
 */
class PassExecuter {
 public:
	virtual void execute(InstRO::Core::PassImplementation *pass) = 0;
};
}
}

#endif
