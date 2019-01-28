#ifndef TEST_LIB_TESTADAPTER_H
#define TEST_LIB_TESTADAPTER_H

#include "instro/core/ConstructSet.h"
#include "instro/core/PassImplementation.h"

#include <algorithm>
#include <fstream>

namespace InstRO {
namespace Test {

/*
 * The class is used to decouple the process of acquiring the data during a test run (done by TestAdapter) and printing
 * the data.
 *
 * Holds two multisets
 * - nodes which were expected but not found
 * - nodes that were found but not expected
 *
 */
class TestSummary {
 public:
	TestSummary(std::string adapterLabel) : lbl(adapterLabel) {}

	void setTestResult(std::multiset<std::string>&& unFound, std::multiset<std::string>&& additionallyMarked,
										 std::set<std::shared_ptr<InstRO::Core::Construct>>&& additionallyMarkedConstructs) {
		unfoundSet = std::move(unFound);
		addMarked = std::move(additionallyMarked);
		addMarkedConstructs = std::move(additionallyMarkedConstructs);
	}

	const std::string& getLabel() const { return lbl; }

	void printResults();

	bool failed() const { return unfoundSet.size() > 0 || addMarked.size() > 0; }

	void assumeFound(const std::multiset<std::string>& identifiers);

 private:
	std::string lbl;
	std::multiset<std::string> unfoundSet;
	std::multiset<std::string> addMarked;
	std::set<std::shared_ptr<InstRO::Core::Construct>> addMarkedConstructs;
};

/*
 * Reads the file at the provided location and fills the expected list according to the given label.
 * Pushes the collected multisets into a TestSummary, which is managed inside the Instrumentor/Factory.
 */
class TestAdapter : public InstRO::Core::PassImplementation {
 public:
	TestAdapter(std::string lab, std::string filename, TestSummary* tr)
			: InstRO::Core::PassImplementation(), label(lab), filename(filename), summary(tr) {}

	void init() override;
	void execute() override;
	void finalize() override;

 protected:
	virtual std::vector<std::string> constructMatchesAnyExpectation(
			const std::string& testIdentifier, std::shared_ptr<InstRO::Core::Construct> construct) const;

	const std::multiset<std::string>& getExpectedItems() const;

 private:
	void checkIfConstructSetMatches(const InstRO::Core::ConstructSet* cs);

	std::multiset<std::string> readExpectedItemsFile();

	std::string label;
	std::string filename;
	TestSummary* summary;
	// To allow for multi occurence we save the marked values in markedValues and have a set_difference at the end
	std::multiset<std::string> expectedItems;
	std::multiset<std::string> markedItems;
	std::multiset<std::string> erroneouslyContainedInConstructSet;
	std::set<std::shared_ptr<InstRO::Core::Construct>> addMarkedConstructs;
};
}	// namespace Test
}	// namespace InstRO

#endif
