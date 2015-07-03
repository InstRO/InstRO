#include "instro/core/Pass.h"
#include "instro/example/ExamplePass.h"

namespace InstRO {
namespace Example {
class ExampleConstructPrinter : public ExamplePass {
 protected:
	InstRO::Pass *inputPass;
	InstRO::Core::ConstructSet outputCS;

 public:
	ExampleConstructPrinter(InstRO::Pass *pass)
			: ExamplePass(InstRO::Core::ChannelConfiguration(pass)), inputPass(pass) {}
	virtual void init(){};
	virtual void execute() override;
	virtual void finalize(){};
	virtual void releaseOutput() { outputCS.clear(); };
	virtual InstRO::Core::ConstructSet *getOutput() { return &outputCS; }
};
}
}