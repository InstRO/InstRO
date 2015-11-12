#ifndef TEST_LIB_TESTADAPTER_H
#define TEST_LIB_TESTADAPTER_H

#include "instro/core/PassImplementation.h"
#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Test {

class TestAdapter : public InstRO::Core::PassImplementation {
 public:
	TestAdapter(InstRO::Pass *input, std::set<std::string> expItems)
			: InstRO::Core::PassImplementation(InstRO::Core::ChannelConfiguration(input)), expectedItems(expItems) {}

	void execute() override;
	void finalize() override;

 private:
	void checkIfConstructSetMatches(InstRO::Core::ConstructSet *cs);

	std::set<std::string> expectedItems;
	std::set<std::string> erroneouslyContainedInConstructSet;
};
}
}

#endif
