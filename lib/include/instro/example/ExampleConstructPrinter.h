#include "instro/core/Pass.h"
#include "instro/example/ExamplePass.h"

namespace InstRO {
namespace Example {
class ExampleConstructPrinter : public ExamplePass {
 protected:
	InstRO::Pass *inputPass;

 public:
	ExampleConstructPrinter(InstRO::Pass *pass)
			: ExamplePass(InstRO::Core::ChannelConfiguration(pass)), inputPass(pass) {}
	virtual void execute() override;
};
}
}
