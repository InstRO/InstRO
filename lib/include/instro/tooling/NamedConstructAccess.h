#ifndef INSTRO_TOOLING_NAMED_CONSTRUCT_ACCESS
#define INSTRO_TOOLING_NAMED_CONSTRUCT_ACCESS

#include <vector>
#include <memory>	// We need shared pointers
#include "instro/core/ConstructSet.h"
#include "instro/utility/exception.h"

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
	char WILDCARDCHAR;														// The used wild card character
};

class NamedConstructAccess {
 public:
	virtual ~NamedConstructAccess() {}
	// CI: what could we match against
	// a) any userdefined identifyer, e.g. variable names, function / method names, labels
	// b) user supplied text, e.g. identifyer, but also contents of strings and comments
	// c) raw source code
	//
	// There is also a difference between
	//    1) topDown - maximal match  (i.e. the first match in a  given AST tree is only reported)
	//    2) bottomUp - minimal match (i.e. the smallest possible match is only reported)
	//    3) every match, i.e. every construct that matches is reported. This WILL result in one match resulting in
	//    multiple constructs

	// After fooling around with the implementation and testing some potential scenarious I consider the top-down, bottom
	// up schematic
	// flawed. In principle, a single selection is sufficient.

	// a) any userdefined symbols, e.g. variable names, function / method names, labels
	virtual InstRO::Core::ConstructSet getConstructsByIdentifierName(Matcher &) = 0;
	// b) contents of strings
	virtual InstRO::Core::ConstructSet getConstructsByUserTextStringMatch(Matcher &) = 0;
	// c) raw source code. First all symbols, then the upward expressions, then the upward statement are matched. Last is
	// the whole function matched.
	virtual InstRO::Core::ConstructSet getConstructsByCodeMatch(Matcher &) {
		raise_exception("Not Implemented");
		return InstRO::Core::ConstructSet();
	}
};
}
}
}
#endif
