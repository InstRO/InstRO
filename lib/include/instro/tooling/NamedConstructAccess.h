#ifndef INSTRO_ANALYSIS_INTERFACE_NAMED_CONSTRUCT_ACCESS
#define INSTRO_ANALYSIS_INTERFACE_NAMED_CONSTRUCT_ACCESS

#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
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

	/** returns the ids in list of the patterns which match */
	//			virtual std::vector<int> match(std::string str, std::list<std::string> list) = 0;
	/** 2013-10-15 JP: New function added. Maybe to replace the std::vector<int> match(...) function */
	//			virtual bool bMatch(std::string str, std::list<std::string> list) = 0;

	/** 2015-07-02 CI: Generic Match function **/
	virtual bool isMatch(std::string str) = 0;
	/** returns a vector of vectors holding the ids in list of the patterns which match */
	//			virtual std::vector<std::vector<int> > match(std::vector<std::string> strList, std::list<std::string> list) =
	// 0;
	/** get the matching string by id */
	//			virtual std::vector<int> getMatchIds() = 0; // XXX 2013-10-08 JP: really reasonable?
};

//#define WILDCARDCHAR '#'
/**
* This class implements a wildcard string matching.
* The '#' character is considered to be the Wildcard which can substitute 0,..,n characters.
* \ingroup Util
* \author Jan-Patrick Lehr
*/
class WildcardedStringMatcher : public Matcher {
 protected:
	std::list<std::string> wildcardStrings;

 public:
	/** Initializes the wild card char to # and sets verbose to false. */
	WildcardedStringMatcher(std::list<std::string> inputWildcardStrings)
			: wildcardStrings(inputWildcardStrings),
				WILDCARDCHAR('#'),
				verbose(false){};	// Todo JP: Maybe we should have here parameters with default values?

	/** returns a vector with the ids of the matching strings in list */
	virtual std::vector<int> match(std::string str, std::list<std::string> list);
	/** returns a boolean if anything matched. The match can be retrieved using the get method */
	virtual bool bMatch(std::string str, std::list<std::string> list);
	virtual bool isMatch(std::string str) override;
	/** returns a vector with vectors of ids of the matching strings in the list. */
	virtual std::vector<std::vector<int> > match(std::vector<std::string> strList, std::list<std::string> list);
	/** Returns a list of integers indicating the position which matched 'str' after one call to match(...) */
	virtual std::vector<int> getMatchIds();
	/** Sets the verbose mode. If verbose is set, a lot of output will be created! */
	void setVerbose(bool verbose);	// enables verbose output
	std::vector<std::string> getMatchingStringList();

 protected:
	std::vector<int> matchingList;	// holds the position of each matching pattern after one call to match(..)

 private:
	std::vector<std::string> matchingStringList;	// holds all matching patterns after one call to match(..)
	bool verbose;																	// Indicates verbosity of the matcher object
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
	virtual std::unique_ptr<InstRO::Core::ConstructSet> getConstructsByIdentifyerName(Matcher &) = 0;
	// b) contents of strings
	virtual std::unique_ptr<InstRO::Core::ConstructSet> getConstructsByUserTextStringMatch(Matcher &) = 0;
	// c) raw source code. First all symbols, then the upward expressions, then the upward statement are matched. Last is
	// the whole function matched.
	virtual std::unique_ptr<InstRO::Core::ConstructSet> getConstructsByCodeMatch(Matcher &) {
#ifdef __EXCEPTIONS
		throw std::string("Not Implemented");
#endif
		return std::unique_ptr<InstRO::Core::ConstructSet>(nullptr);
	}
};
}
}
}
#endif
