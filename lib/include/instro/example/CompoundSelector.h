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
				int compareMode();
			public:
				typedef enum {
					CO_Or,
					CO_And,
					CO_XOr
				} CompoundOperationType;
				CompoundSelector(Pass* inputA, Pass* inputB, CompoundOperationType Operation)
					: ExamplePass(InstRO::Core::ChannelConfiguration(inputA, inputB)){};
				CompoundSelector(Pass* inputA, bool invertPassA, Pass* inputB, bool invertPassB, CompoundOperationType Operation)
					: ExamplePass(InstRO::Core::ChannelConfiguration(inputA, inputB)){};
				void init() override {};
				void execute() override;
				void finalize() override{};
				void releaseOutput(){ passOutput->clear(); };
				InstRO::Core::ConstructSet* getOutput() override { return passOutput.get(); }
			};
		}	// End namespace Selectors
	}	// End namespace Rose
}	// End namepsace InstRO
#endif