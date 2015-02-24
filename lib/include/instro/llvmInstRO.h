#ifndef INSTRO_LLVM_INTERFACE_H
#define INSTRO_LLVM_INTERFACE_H

#include "instro/interface.h"

/*
 * Within the LLVM namespace lives the implementation of the Clang and LLVM
 * dependent
 * components and infrastructure.
 */
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

		/** An LLVM Passes factory */
		class PassFactory : public ::InstRO::PassFactory {
		 public:
			Pass *createCygProfileAdapterPass(Pass *inputPass);
		};

		/** InstRO for LLVM */
		class InstRO : public ::InstRO::InstRO {
		 public:
			PassFactory *getFactory();
		};
	}
}
#endif
