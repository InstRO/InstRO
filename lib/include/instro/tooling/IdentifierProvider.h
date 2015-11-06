#ifndef LIB_INCLUDE_INSTRO_TOOLING_IDENTIFIERPROVIDER_H
#define LIB_INCLUDE_INSTRO_TOOLING_IDENTIFIERPROVIDER_H

#include "instro/core/ConstructSet.h"

#include <map>

namespace InstRO {
namespace Tooling {

/**
 * Provides an interface to retrieve a mapping from a unique identifier to a human readable representation of the
 * contents in a ConstructSet
 * */
class IdentifierProvider {
 public:
	// Create and return a mapping from a ConstructSet to human readable identifiers.
	std::map<size_t, std::string> getIdentifierMap(Core::ConstructSet *cs) {
		// we want to have access to single constructs
		using ROCompI = InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface;
		ROCompI roci(cs);
		std::map<size_t, std::string> m;
		for (const auto &c : roci) {
			m.insert({c->getID(), c->getIdentifier()});
		}
		return m;
	}
};
}
}

#endif
