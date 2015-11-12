#include "TestAdapter.h"
#include "instro/utility/Logger.h"
#include "instro/tooling/IdentifierProvider.h"

void InstRO::Test::TestAdapter::execute() {
	auto cfg = getChannelConfig();
	for (auto p : cfg.getPasses()) {
		checkIfConstructSetMatches(getInput(p));
	}
}

void InstRO::Test::TestAdapter::checkIfConstructSetMatches(InstRO::Core::ConstructSet *cs) {
	InstRO::Tooling::IdentifierProvider ip;
	auto idMap = ip.getIdentifierMap(cs);

	for (const auto idPair : idMap) {
		// TODO before we find something we need to prepare the identifier
		// as we don't want to match full paths.

		// erase returns number of elements removed
		if (expectedItems.erase(idPair.second) > 0) {
		} else {
			erroneouslyContainedInConstructSet.insert(idPair.second);
		}
	}
}

void InstRO::Test::TestAdapter::finalize() {
	logIt(ERROR) << "UNFOUND items " << expectedItems.size() << "\n";
	for (const auto i : expectedItems) {
		logIt(INFO) << i << "\n";
	}

	logIt(ERROR) << "WRONGLY MARKED " << erroneouslyContainedInConstructSet.size() << "\n";
	for (const auto i : erroneouslyContainedInConstructSet) {
		logIt(INFO) << i << "\n";
	}

	// XXX Make this exit with different return codes for different errors
	if(expectedItems.size() > 0 || erroneouslyContainedInConstructSet.size() > 0){
		exit(-1);
	}
}
