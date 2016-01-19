#define INSTRO_LOG_LEVEL ERROR

#include "TestAdapter.h"
#include "instro/utility/Logger.h"
#include "instro/tooling/IdentifierProvider.h"

void InstRO::Test::TestSummary::printResults() {
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
		logIt(DEBUG) << "=== DEBUG ===\n== Dumping the wrongly marked constructs using \"toString\" ==\n=== ===== ==="
								 << std::endl;
		for (const auto pC : addMarkedConstructs) {
			logIt(DEBUG) << pC->toString() << "\n";
		}
		logIt(DEBUG) << "=====\n== Done ==\n=====" << std::endl;
	}
}

void InstRO::Test::TestAdapter::init() { expectedItems = readExpectedItemsFile(); }

void InstRO::Test::TestAdapter::execute() {
	logIt(INFO) << "Running TestAdapter with label \"" << label << "\"" << std::endl;
	//	auto cfg = getChannelConfig();
	//	for (auto p : cfg.getPasses()) {
	checkIfConstructSetMatches(getInput(0));
	//	}
}

void InstRO::Test::TestAdapter::checkIfConstructSetMatches(const InstRO::Core::ConstructSet *cs) {
	auto idMap = InstRO::Tooling::IdentifierProvider::getIdentifierMap(cs);

	for (const auto idPair : idMap) {
		std::string keyVal(idPair.second);
		std::string testString(keyVal.substr(keyVal.rfind("/") + 1));

		markedItems.insert(testString);

		logIt(DEBUG) << "Marking " << keyVal << " as selected" << std::endl;

		if (expectedItems.find(testString) != expectedItems.end()) {
		} else {
			InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface roci(cs);
			auto cPos = std::find_if(roci.begin(), roci.end(), [&](const std::shared_ptr<InstRO::Core::Construct> c) {
				return c->getID() == idPair.first;
			});

			addMarkedConstructs.insert(*cPos);
			erroneouslyContainedInConstructSet.insert(testString);
		}
	}
}

void InstRO::Test::TestAdapter::finalize() {
	/*
	 * We want to have all nodes which were found but not expected as well as all nodes which were not found but expected.
	 * The fun part is, we need to have them in the correct amount, as we are dealing with multisets here.
	 */
	std::multiset<std::string> unfoundSet;
	std::set_difference(expectedItems.begin(), expectedItems.end(), markedItems.begin(), markedItems.end(),
											std::inserter(unfoundSet, unfoundSet.begin()));

	std::multiset<std::string> tempAdditionals;
	std::set_difference(markedItems.begin(), markedItems.end(), expectedItems.begin(), expectedItems.end(),
											std::inserter(tempAdditionals, tempAdditionals.begin()));

	std::multiset<std::string> addNodes;
	std::set_union(tempAdditionals.begin(), tempAdditionals.end(), erroneouslyContainedInConstructSet.begin(),
								 erroneouslyContainedInConstructSet.end(), std::inserter(addNodes, addNodes.begin()));

	summary->setTestResult(std::move(unfoundSet), std::move(addNodes), std::move(addMarkedConstructs));
}

/* builds a multiset of expected items */
std::multiset<std::string> InstRO::Test::TestAdapter::readExpectedItemsFile() {
	std::multiset<std::string> ei;

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
