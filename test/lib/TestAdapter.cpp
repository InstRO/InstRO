#include "TestAdapter.h"
#include "instro/utility/Logger.h"
#include "instro/tooling/IdentifierProvider.h"


void InstRO::Test::TestReporter::printResults(){

	if (unfoundSet.size() > 0 || addMarked.size() > 0) {
		logIt(ERROR) << "Adapter: " << lbl << std::endl;
	}

	if (unfoundSet.size() > 0) {
		logIt(ERROR) << "UNFOUND items " << unfoundSet.size() << "\n";
		for (const auto i : unfoundSet) {
			logIt(ERROR) << i << "\n";
		}
	}

	if (addMarked.size() > 0) {
		logIt(ERROR) << "WRONGLY MARKED " << addMarked.size() << "\n";
		for (const auto i : addMarked) {
			logIt(ERROR) << i << "\n";
		}
	}
}


void InstRO::Test::TestAdapter::init() { expectedItems = readExpectedItemsFile(); }

void InstRO::Test::TestAdapter::execute() {
	logIt(INFO) << "Running TestAdapter with labeled \"" << label << "\"" << std::endl;
	auto cfg = getChannelConfig();
	for (auto p : cfg.getPasses()) {
		checkIfConstructSetMatches(getInput(p));
	}
}

void InstRO::Test::TestAdapter::checkIfConstructSetMatches(InstRO::Core::ConstructSet *cs) {
	InstRO::Tooling::IdentifierProvider ip;
	auto idMap = ip.getIdentifierMap(cs);

	for (const auto idPair : idMap) {
		std::string keyVal(idPair.second);
		std::string testString(keyVal.substr(keyVal.rfind("/") + 1));
		markedItems.insert(testString);

		// erase returns number of elements removed
		if (expectedItems.find(testString) != expectedItems.end()) {
		} else {
			erroneouslyContainedInConstructSet.insert(idPair.second);
		}
	}
}

void InstRO::Test::TestAdapter::finalize() {
	std::set<std::string> unfoundSet;
	std::set_difference(expectedItems.begin(), expectedItems.end(), markedItems.begin(), markedItems.end(),
											std::inserter(unfoundSet, unfoundSet.begin()));

	reporter->setTestResult(std::move(unfoundSet), std::move(erroneouslyContainedInConstructSet));
}

// builds a set of expected items
std::set<std::string> InstRO::Test::TestAdapter::readExpectedItemsFile() {
	std::set<std::string> ei;

	std::ifstream inFile(filename);

	bool labelApplies(false);
	while (inFile.good()) {
		std::string line;
		std::getline(inFile, line);

		if (line.front() == '+') {
			labelApplies = (label == line.substr(1));
			continue;
		}

		if (labelApplies && !line.empty()) {
			ei.insert(line);
		}
	}

	return ei;
}
