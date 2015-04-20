#ifndef INSTRO_LLVM_CONSTRUCTSET_H
#define INSTRO_LLVM_CONSTRUCTSET_H

/* LLVM related includes */
#include "llvm/IR/Value.h"

/* InstRO related includes */
#include "instro/core/ConstructSet.h"

namespace InstRO {

namespace LLVM {
namespace Core {
/*
 * A Construct Set stores the selected nodes. This is implementation
 * specific
 * and one generally needs to cast to the specific type
 */
class LLVMConstructSet : public InstRO::Core::ConstructSet {
 public:
	std::vector<llvm::Value *> &getConstructSet() { return constructs; }

 private:
	// Value is LLVMs most abstract type
	std::vector<llvm::Value *> constructs;
};
}
}
}

#endif
