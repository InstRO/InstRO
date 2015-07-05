#ifndef ROSE_COMPOUNDSELECTORS_H
#define ROSE_COMPOUNDSELECTORS_H
#include <vector>
#include <string>
#include <memory>
#include "instro/example/ExamplePass.h"
#include "instro/core/Pass.h"								 // Pass
#include "instro/core/PassImplementation.h"	// ChannelConfiguration

namespace InstRO {
	namespace Example {
		namespace Selectors {

			class CompoundSelector : public InstRO::Example::ExamplePass {
			protected:
				std::unique_ptr<InstRO::Core::ConstructSet> passOutput;
				// CI: I see different compare modes:
				// 1) Strict: a pair only matches, if both are at the same level and the Operation results in true
				// 2) Parent-child: one of the operands is the "AST"-Parent of the child node
				// 3) Common parent: both nodes are comparable if they share a common parent up to construct level-x
				int compareMode();
				void strictCompare();
				Pass * firstArg, *secondArg;
			public:
				typedef enum {
					CO_Or,
					CO_And,
					CO_XOr
				} CompoundOperationType;
				CompoundSelector(Pass* inputA, Pass* inputB, CompoundOperationType Operation)
					: ExamplePass(InstRO::Core::ChannelConfiguration(inputA, inputB)), firstArg(inputA),secondArg(inputB), compoundOperation(Operation){};
				CompoundSelector(Pass* inputA, bool invertPassA, Pass* inputB, bool invertPassB, CompoundOperationType Operation)
					: ExamplePass(InstRO::Core::ChannelConfiguration(inputA, inputB)), firstArg(inputA), secondArg(inputB), compoundOperation(Operation){};
				void init() override {};
				void execute() override;
				void finalize() override{};
				void releaseOutput(){ passOutput->clear(); };
				InstRO::Core::ConstructSet* getOutput() override { return passOutput.get(); }
			protected:
				bool isStrict(){ return true; };
				bool isParentChild(){ return true; };
				bool isCommonParent(){ return true; };
					CompoundOperationType compoundOperation;

					CompoundOperationType getCompoundOperationType(){ return compoundOperation; }
			};
		}	// End namespace Selectors
	}	// End namespace Rose
}	// End namepsace InstRO
#endif