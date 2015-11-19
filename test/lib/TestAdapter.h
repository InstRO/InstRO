#ifndef TEST_LIB_TESTADAPTER_H
#define TEST_LIB_TESTADAPTER_H

#include "instro/core/PassImplementation.h"
#include "instro/core/ConstructSet.h"

#include <fstream>
#include <algorithm>

namespace InstRO {
namespace Test {

class TestSummary {
 public:
	 TestSummary(std::string adapterLabel) : lbl(adapterLabel){}

	void setTestResult(std::set<std::string> &&unFound, std::set<std::string> &&additionallyMarked) {
		unfoundSet.insert(unFound.begin(), unFound.end());
		addMarked.insert(additionallyMarked.begin(), additionallyMarked.end());
	}

	void printResults();

	bool failed() { return unfoundSet.size() > 0 || addMarked.size() > 0; }

 private:
	std::string lbl;
	std::set<std::string> unfoundSet;
	std::set<std::string> addMarked;
};

class TestAdapter : public InstRO::Core::PassImplementation {
 public:
	TestAdapter(InstRO::Pass *input, std::string lab, std::string filename, TestSummary *tr)
			: InstRO::Core::PassImplementation(InstRO::Core::ChannelConfiguration(input)), label(lab), filename(filename), summary(tr) {}

	void init() override;
	void execute() override;
	void finalize() override;

 private:
	void checkIfConstructSetMatches(InstRO::Core::ConstructSet *cs);

	std::set<std::string> readExpectedItemsFile();

	std::string label;
	std::string filename;
	TestSummary *summary;
	// To allow for multi occurence we save the marked values in markedValues and have a set_difference at the end
	std::set<std::string> expectedItems;
	std::set<std::string> markedItems;
	std::set<std::string> erroneouslyContainedInConstructSet;
};
}
}

#endif
