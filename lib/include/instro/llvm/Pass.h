#ifndef INSTRO_LLVM_PASS_H
#define INSTRO_LLVM_PASS_H

#include <map>

/* Do we need this for the top-level Pass class? */
#include "interface.h"

/* Gives us an LLVM Construct Set */
#include "llvm/core/ConstructSet.h"

namespace InstRO {
	namespace LLVM {
		/** Common base class for LLVM Passes */
		class Pass : public ::InstRO::Pass {
		 public:
			/* returns true if an already attached construct set was overriden, else
			 * false*/
			bool overrideInput(Pass *p, ConstructSet *cs);

		 protected:
			ConstructSet *getInput(Pass *p);

		 private:
			std::map<Pass *, ConstructSet *> inputOverride;
		};
	}
}
#endif

