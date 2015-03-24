#ifndef INSTRO_LLVM_CONSTRUCTSETPASSING_H
#define INSTRO_LLVM_CONSTRUCTSETPASSING_H

#include "instro/llvm/core/ConstructSet.h"

namespace InstRO {
namespace LLVM {
namespace Core {
class ConstructSetPassing {
 public:
	InstRO::LLVM::Core::LLVMConstructSet* getConstructSet() { return &cs; };

 protected:
	void put(llvm::Value* val) { cs.getConstructSet().push_back(val); }

 private:
	LLVMConstructSet cs;
};
}
}
}

#endif
