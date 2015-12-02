#ifndef INSTRO_TOOLING_NAMED_CONSTRUCT_ACCESS_H
#define INSTRO_TOOLING_NAMED_CONSTRUCT_ACCESS_H

#include <vector>
#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Tooling {
namespace NamedConstructAccess {

/**
* This class is an interface for a matching algorithm to match named constructs against
* patterns potentially holding wildcards. The Matcher contains all information for the matching process, i.e. user
* defined strings , etc.
* \ingroup Tooling
* \author Jan-Patrick Lehr, Christian Iwainsky
*/
class Matcher {
 public:
	Matcher(){};
	virtual ~Matcher() {}

	virtual bool isMatch(std::string candidate, std::string pattern) = 0;

	virtual bool isMatch(std::string candidate) = 0;
};

/**
* This class implements a wildcard string matching.
* The '#' character is considered to be the Wildcard which can substitute 0,..,n characters.
*
* \author Jan-Patrick Lehr
*/
class WildcardedStringMatcher : public Matcher {
 public:
	/** Initializes the wild card char to # */
	WildcardedStringMatcher() : WILDCARDCHAR('#') {}

	/** Initializes wild card char to '#' and used patterns-list with patternList */
	WildcardedStringMatcher(std::vector<std::string> patternList) : patterns(patternList), WILDCARDCHAR('#') {}

	/** Checks candidate against pattern */
	virtual bool isMatch(std::string candidate, std::string pattern) override;

	/** Checks the candidate against all patterns, short-circuits on first match */
	virtual bool isMatch(std::string candidate) override;

 protected:
	bool bMatch(std::string str, std::string pattern);

 private:
	std::vector<std::string> patterns;
	const char WILDCARDCHAR;	// The used wild card character
};

/**
 * \brief Get all constructs that satisfy the given name matcher
 */
class NamedConstructAccess {
 public:
	virtual ~NamedConstructAccess() {}

	// a) any userdefined symbols, e.g. variable names, function / method names, labels
	virtual InstRO::Core::ConstructSet getConstructsByIdentifierName(Matcher &) = 0;

	// b) + contents of strings and comments
	virtual InstRO::Core::ConstructSet getConstructsByUserTextStringMatch(Matcher &) {
		throw std::string("NamedConstructAccess::getConstructsByUserTextStringMatch() is not implemented.");
	}

	// c) raw source code
	virtual InstRO::Core::ConstructSet getConstructsByCodeMatch(Matcher &) {
		throw std::string("NamedConstructAccess::getConstructsByCodeMatch() is not implemented.");
	}
};
}
}
}

#endif
