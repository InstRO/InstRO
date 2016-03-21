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

void InstRO::Test::TestSummary::assumeFound(const std::multiset<std::string> &identifiers) {
	for (auto identifier : identifiers) {
		auto range = unfoundSet.equal_range(identifier);
		unfoundSet.erase(range.first, range.second);
	}
}

void InstRO::Test::TestAdapter::init() { expectedItems = readExpectedItemsFile(); }

void InstRO::Test::TestAdapter::execute() {
	logIt(INFO) << "Running TestAdapter with label \"" << label << "\"" << std::endl;
	checkIfConstructSetMatches(getInput(0));
}

const std::multiset<std::string> &InstRO::Test::TestAdapter::getExpectedItems() const { return expectedItems; }

std::vector<std::string> InstRO::Test::TestAdapter::constructMatchesAnyExpectation(
		const std::string &testIdentifier, std::shared_ptr<InstRO::Core::Construct> construct) const {
	if (expectedItems.find(testIdentifier) != expectedItems.end()) {
		return std::vector<std::string>{testIdentifier};
	} else {
		return std::vector<std::string>();
	}
}

void InstRO::Test::TestAdapter::checkIfConstructSetMatches(const InstRO::Core::ConstructSet *cs) {
	auto idMap = InstRO::Tooling::IdentifierProvider::getIdentifierMap(cs);

	for (const auto idPair : idMap) {
		std::string keyVal(idPair.second);
		std::string testString(keyVal.substr(keyVal.rfind("/") + 1));

		logIt(DEBUG) << "Marking " << keyVal << " as selected" << std::endl;

		InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface roci(cs);
		auto cPos = std::find_if(roci.begin(), roci.end(), [&](const std::shared_ptr<InstRO::Core::Construct> c) {
			return c->getID() == idPair.first;
		});

		auto matchedIdentifiers = constructMatchesAnyExpectation(testString, *cPos);
		if (matchedIdentifiers.empty()) {
			addMarkedConstructs.insert(*cPos);
			erroneouslyContainedInConstructSet.insert(matchedIdentifiers.begin(), matchedIdentifiers.end());
		}
		// more sophisticated matching schemes may manually change the identifier to the corresponding item
		markedItems.insert(matchedIdentifiers.begin(), matchedIdentifiers.end());
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
