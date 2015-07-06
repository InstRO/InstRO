#include "instro/core/Pass.h"
#include "instro/rose/core/RosePassImplementation.h"

namespace InstRO {
namespace Rose {
namespace Adapter {
class ExampleConstructPrinter : public RosePassImplementation {
protected:
	InstRO::Pass *inputPass;
	InstRO::Core::ConstructSet outputCS;

public:
	ExampleConstructPrinter(InstRO::Pass *pass)
		: RosePassImplementation(InstRO::Core::ChannelConfiguration(pass)), inputPass(pass) {}
	virtual void init(){};
	virtual void execute() override;
	virtual void finalize(){};
	virtual void releaseOutput() { outputCS.clear(); };
	virtual InstRO::Core::ConstructSet *getOutput() { return &outputCS; }
};
}
}
}