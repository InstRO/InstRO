#include "instro/tooling/IdentifierProvider.h"

#include "instro/utility/Logger.h"

#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>


namespace InstRO {
namespace Tooling {
namespace IdentifierProvider {

size_t getID(std::shared_ptr<Core::Construct> construct) { return construct->getID(); }

std::string getIdentifier(std::shared_ptr<Core::Construct> construct) { return construct->getIdentifier(); }

// Create and return a mapping from a ConstructSet to human readable identifiers.
std::map<size_t, std::string> getIdentifierMap(const Core::ConstructSet *cs) {
	// we want to have access to single constructs
	using ROCompI = InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface;
	ROCompI roci(cs);
	std::map<size_t, std::string> m;
	for (const auto &c : roci) {
		m[getID(c)] = getIdentifier(c);
	}
	return m;
}

void exportToNMFile(std::map<size_t, std::string> mapping, std::string filename) {
	std::stringstream ss;
	const std::string fragment("Fragment");
	for (const auto p : mapping) {
		auto i = p.second;
		if (i.find(fragment) != std::string::npos) {
			continue;
		}
		ss << std::hex << std::noshowbase << std::setfill('0') << std::setw(16) << p.first;
		ss << " T " << i << "\n";
	}

	std::ofstream outFile(filename.c_str());
	outFile << ss.str();
	outFile.close();
}
}
}
}

