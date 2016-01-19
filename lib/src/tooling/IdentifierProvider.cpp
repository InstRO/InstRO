#include "instro/tooling/IdentifierProvider.h"

namespace InstRO {
namespace Tooling {
namespace IdentifierProvider {

	size_t getID(std::shared_ptr<Core::Construct> construct) {
		return construct->getID();
	}

	std::string getIdentifier(std::shared_ptr<Core::Construct> construct) {
		return construct->getIdentifier();
	}

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

}
}
}


