#ifndef TEST_LIB_TESTADAPTER_H
#define TEST_LIB_TESTADAPTER_H

#include "instro/core/PassImplementation.h"
#include "instro/core/ConstructSet.h"

#include <fstream>
#include <algorithm>

namespace InstRO {
namespace Test {

class TestAdapter : public InstRO::Core::PassImplementation {
 public:
	TestAdapter(InstRO::Pass *input, std::string lab, std::string filename)
			: InstRO::Core::PassImplementation(InstRO::Core::ChannelConfiguration(input)), label(lab), filename(filename) {}

	void init() override;
	void execute() override;
	void finalize() override;

 private:
	void checkIfConstructSetMatches(InstRO::Core::ConstructSet *cs);

	std::set<std::string> readExpectedItemsFile();

	std::string label;
	std::string filename;
	// To allow for multi occurence we save the marked values in markedValues and have a set_difference at the end
	std::set<std::string> expectedItems;
	std::set<std::string> markedItems;
	std::set<std::string> erroneouslyContainedInConstructSet;
};
}
}

#endif
