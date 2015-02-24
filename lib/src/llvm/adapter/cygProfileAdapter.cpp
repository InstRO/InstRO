#include "cygProfileAdapter.h"


char CygProfileAdapter::ID = 0;

CygProfileAdapter::CygProfileAdapter(::InstRO::LLVM::Pass *inputSel)
    : exitName("__cyg_profile_func_exit"),
      entryName("__cyg_profile_func_enter") {}

bool CygProfileAdapter::doInitialization(llvm::Module &m) {
  mod = &m;
  llvm::Function *func = buildFunction(exitName);
  mod->getOrInsertFunction(exitName, func->getFunctionType());
  func = buildFunction(entryName);
  mod->getOrInsertFunction(entryName, func->getFunctionType());
}

bool CygProfileAdapter::runOnFunction(llvm::Function &f) {
  CashesSelector &defSel = getAnalysis<CashesSelector>();
  auto res = std::find(std::begin(defSel.selectionSet), std::end(defSel.selectionSet), &f);
  if(res != std::end(defSel.selectionSet)) {
    entryFunc = buildEntryCall(f);
    for(auto &bb : f.getBasicBlockList()) {
      llvm::BasicBlock::InstListType &instructions = bb.getInstList();
      for(auto &i : instructions) {
        if(llvm::isa<llvm::ReturnInst>(i)) {
          llvm::ReturnInst *ti = llvm::dyn_cast<llvm::ReturnInst>(&i);
          exitFunc = buildExitCall(f, ti);
        }
      }
    }
    // we insert the call to the entry function
    llvm::BasicBlock &bb = f.getEntryBlock();
    bb.getInstList().insert(bb.getFirstInsertionPt(), entryFunc);
    return true;
  } else {
    return false;
  }
}

llvm::CallInst *CygProfileAdapter::buildExitCall(llvm::Function &f, llvm::ReturnInst *ri) {
  return buildTCall(f, llvm::Twine(exitName), ri);
}

llvm::CallInst *CygProfileAdapter::buildEntryCall(llvm::Function &f) {
  return buildTCall(f, llvm::Twine(entryName), nullptr);
}

llvm::Function *CygProfileAdapter::buildFunction(std::string name) {
  llvm::PointerType *voidPtr = llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(mod->getContext()));
  std::vector<llvm::Type *> argTypes(2, voidPtr);
  llvm::ArrayRef<llvm::Type *> argsT(argTypes);

  // We get ourselves the pointer to the function type
  llvm::FunctionType *fType = llvm::FunctionType::get(llvm::Type::getVoidTy(mod->getContext()), argsT,
                              false);

  // Creates a new llvm Function object, that can be used inside a call instruction
  llvm::Function *func = llvm::Function::Create(fType, llvm::GlobalValue::ExternalLinkage,
                         llvm::Twine(name));
  return func;
}

llvm::CallInst *CygProfileAdapter::buildTCall(llvm::Function &f, llvm::Twine &&name,
    llvm::Instruction *insertBefore) {
  // constructs a ptr to void type in the general address space
  llvm::PointerType *voidPtr = llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(mod->getContext()));
  std::vector<llvm::Type *> argTypes(2, voidPtr);
  llvm::ArrayRef<llvm::Type *> argsT(argTypes);

  llvm::FunctionType *fType = llvm::FunctionType::get(llvm::Type::getVoidTy(mod->getContext()), argsT,
                              false);
  // Get a pointer to the function
  llvm::Constant *castExpr = llvm::ConstantExpr::getPointerCast(&f, voidPtr);
  llvm::Constant *callsite = llvm::ConstantPointerNull::get(voidPtr);

  // build a vector holding the call argument list
  std::vector<llvm::Value *> a;//(2, castExpr);
  a.push_back(castExpr);
  a.push_back(callsite);
  llvm::ArrayRef< llvm::Value *> args(a);
  llvm::Twine n("");
  llvm::Constant *func = mod->getOrInsertFunction(name.getSingleStringRef(), fType);

  llvm::CallInst *callInst = llvm::CallInst::Create(func, args, n, insertBefore);
  return callInst;
}


