#ifndef INSTRO_LLVM_CONSTRUCTSET_H
#define INSTRO_LLVM_CONSTRUCTSET_H

/* LLVM related includes */
#include "llvm/IR/Value.h"

/* InstRO related includes */
#include "interface.h"

namespace InstRO {

namespace LLVM {
/*
 * A Construct Set stores the selected nodes. This is implementation
 * specific
 * and one generally needs to cast to the specific type
 */
class ConstructSet : public ::InstRO::ConstructSet {
 public:
	std::vector<llvm::Value *> &get();

 private:
	// Value is LLVMs most abstract type
	std::vector<llvm::Value *> constructs;
};
}
}

#endif
