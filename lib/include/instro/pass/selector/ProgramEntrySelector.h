#include "instro/core/PassImplementation.h"

namespace InstRO {
namespace Selector {

class ProgramEntrySelector : public InstRO::Core::PassImplementation {
protected:
	std::unique_ptr<Core::ConstructSet> output;
public:
	ProgramEntrySelector()
		: PassImplementation(Core::ChannelConfiguration()){};
	virtual void init() override;
	virtual void execute() override;
	virtual void finalize() override;
	virtual void releaseOutput() override;
	virtual Core::ConstructSet *getOutput() override;
protected:
};


}
}
