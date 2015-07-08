#include "instro/core/PassImplementation.h"


namespace InstRO {
namespace Selectors {
	class CallPathSelector : public InstRO::Core::PassImplementation {
 protected:
	Pass *fromPass, *toPass;
	CallPathSelector() = delete;

 public:
	CallPathSelector(Pass *from, Pass *to)
		: InstRO::Core::PassImplementation(Core::ChannelConfiguration(from, to)), fromPass(from), toPass(to){};
	virtual void init() override;
	virtual void execute() override;
	virtual void finalize() override;
	virtual void releaseOutput() override;
	virtual Core::ConstructSet *getOutput() override;

 protected:
	// InstRO::Example::output;
};

}	// namespace Selectors
}	// namespace InstRO