#ifndef INSTRO_TOOLING_IDENTIFIERPROVIDER_H
#define INSTRO_TOOLING_IDENTIFIERPROVIDER_H

#include "instro/core/ConstructSet.h"

#include <map>
#include <string>

namespace InstRO {
namespace Tooling {

/**
 * Provides an interface to retrieve a mapping from a unique identifier to a human readable representation of the
 * contents in a ConstructSet
 * */
namespace IdentifierProvider {

	size_t getID(std::shared_ptr<Core::Construct> construct);

	std::string getIdentifier(std::shared_ptr<Core::Construct> construct);
	// Create and return a mapping from a ConstructSet to human readable identifiers.
	std::map<size_t, std::string> getIdentifierMap(const Core::ConstructSet *cs);
}
}
}

#endif
