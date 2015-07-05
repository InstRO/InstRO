//#include "CygProfileAdapterT.h"

template <typename T>
char InstRO::LLVM::CygProfileAdapterT<T>::ID = 0;

template <typename T>
InstRO::LLVM::CygProfileAdapterT<T>::CygProfileAdapterT()
		: llvm::FunctionPass(ID),
			pn("CygProfileAdapterT"),
			exitName("__cyg_profile_func_exit"),
			entryName("__cyg_profile_func_enter"),
			inputSelector(nullptr) {
	llvm::errs() << "Running construction " << inputSelector << "\n";
}

template <typename T>
InstRO::LLVM::CygProfileAdapterT<T>::CygProfileAdapterT(T *inputSel)
		: llvm::FunctionPass(ID),
			pn("CygProfileAdapterT"),
			exitName("__cyg_profile_func_exit"),
			entryName("__cyg_profile_func_enter"),
			inputSelector(inputSel) {
	llvm::errs() << "Running construction " << inputSelector << "\n";
}

template <typename T>
void InstRO::LLVM::CygProfileAdapterT<T>::getAnalysisUsage(llvm::AnalysisUsage &info) const {
	info.addRequired<TemplT>();
}

template <typename T>
bool InstRO::LLVM::CygProfileAdapterT<T>::doInitialization(llvm::Module &m) {
	llvm::errs() << "Running initialization\n";
	mod = &m;
	llvm::Function *func = buildFunction(exitName);
	mod->getOrInsertFunction(exitName, func->getFunctionType());
	func = buildFunction(entryName);
	mod->getOrInsertFunction(entryName, func->getFunctionType());
}

template <typename T>
bool InstRO::LLVM::CygProfileAdapterT<T>::runOnFunction(llvm::Function &f) {
	// This method is implemented in InstRO::LLVM::Pass to be able
	// to abstract away a map, which is used by the pass manager to maybe
	// fool the pass requesting the construct set.
	//	auto cs = inputSelector->getConstructSet();
	auto cs = getAnalysis<TemplT>().getConstructSet();
	auto constructs = cs->getConstructSet();
	std::cout << constructs.size() << std::endl;
	//	for(auto &c : constructs){
	//		std::cout << c << " == " << &f << std::endl;
	//	}
	// If the function was marked for adaption
	auto res = std::find(std::begin(constructs), std::end(constructs), &f);
	// we build the instrumentation
	if (res != std::end(constructs)) {
		std::cout << "Adaption!" << std::endl;
		entryFunc = buildEntryCall(f);
		for (auto &bb : f.getBasicBlockList()) {
			llvm::BasicBlock::InstListType &instructions = bb.getInstList();
			for (auto &i : instructions) {
				if (llvm::isa<llvm::ReturnInst>(i)) {
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

template <typename T>
llvm::CallInst *InstRO::LLVM::CygProfileAdapterT<T>::buildExitCall(llvm::Function &f, llvm::ReturnInst *ri) {
	return buildTCall(f, llvm::Twine(exitName), ri);
}

template <typename T>
llvm::CallInst *InstRO::LLVM::CygProfileAdapterT<T>::buildEntryCall(llvm::Function &f) {
	return buildTCall(f, llvm::Twine(entryName), nullptr);
}

template <typename T>
llvm::Function *InstRO::LLVM::CygProfileAdapterT<T>::buildFunction(std::string name) {
	llvm::PointerType *voidPtr = llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(mod->getContext()));
	std::vector<llvm::Type *> argTypes(2, voidPtr);
	llvm::ArrayRef<llvm::Type *> argsT(argTypes);

	// We get ourselves the pointer to the function type
	llvm::FunctionType *fType = llvm::FunctionType::get(llvm::Type::getVoidTy(mod->getContext()), argsT, false);

	// Creates a new llvm Function object, that can be used inside a call
	// instruction
	llvm::Function *func = llvm::Function::Create(fType, llvm::GlobalValue::ExternalLinkage, llvm::Twine(name));
	return func;
}

template <typename T>
llvm::CallInst *InstRO::LLVM::CygProfileAdapterT<T>::buildTCall(llvm::Function &f, llvm::Twine &&name,
																																llvm::Instruction *insertBefore) {
	// constructs a ptr to void type in the general address space
	llvm::PointerType *voidPtr = llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(mod->getContext()));
	std::vector<llvm::Type *> argTypes(2, voidPtr);
	llvm::ArrayRef<llvm::Type *> argsT(argTypes);

	llvm::FunctionType *fType = llvm::FunctionType::get(llvm::Type::getVoidTy(mod->getContext()), argsT, false);
	// Get a pointer to the function
	llvm::Constant *castExpr = llvm::ConstantExpr::getPointerCast(&f, voidPtr);
	llvm::Constant *callsite = llvm::ConstantPointerNull::get(voidPtr);

	// build a vector holding the call argument list
	std::vector<llvm::Value *> a;	//(2, castExpr);
	a.push_back(castExpr);
	a.push_back(callsite);
	llvm::ArrayRef<llvm::Value *> args(a);
	llvm::Twine n("");
	llvm::Constant *func = mod->getOrInsertFunction(name.getSingleStringRef(), fType);

	llvm::CallInst *callInst = llvm::CallInst::Create(func, args, n, insertBefore);
	return callInst;
}
