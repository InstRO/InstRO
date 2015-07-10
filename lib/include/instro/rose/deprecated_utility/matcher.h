#ifndef INSTRUMENTOR_MATCHER_H__
#define INSTRUMENTOR_MATCHER_H__
#include <iostream>
#include <list>
#include <string>
#include <vector>

namespace InstRO {
/**
 * This class is an interface for a matching algorithm to match function names against
 * patterns potentially holding wildcards.
 * \ingroup Util
 * \author Jan-Patrick Lehr
 */
class Matcher {
 public:
	virtual ~Matcher() {}

	/** returns the ids in list of the patterns which match */
	virtual std::vector<int> match(std::string str, std::list<std::string> list) = 0;
	/** 2013-10-15 JP: New function added. Maybe to replace the std::vector<int> match(...) function */
	virtual bool bMatch(std::string str, std::list<std::string> list) = 0;
	/** returns a vector of vectors holding the ids in list of the patterns which match */
	virtual std::vector<std::vector<int> > match(std::vector<std::string> strList, std::list<std::string> list) = 0;
	/** get the matching string by id */
	virtual std::vector<int> getMatchIds() = 0;	// XXX 2013-10-08 JP: really reasonable?
};

//#define WILDCARDCHAR '#'

/**
 * This class implements a wildcard string matching.
 * The '#' character is considered to be the Wildcard which can substitute 0,..,n characters.
 * \ingroup Util
 * \author Jan-Patrick Lehr
 */
class WildcardedStringMatcher : public Matcher {
 public:
	/** Initializes the wild card char to # and sets verbose to false. */
	WildcardedStringMatcher()
			: WILDCARDCHAR('#'), verbose(false){};	// Todo JP: Maybe we should have here parameters with default values?

	/** returns a vector with the ids of the matching strings in list */
	virtual std::vector<int> match(std::string str, std::list<std::string> list);
	/** returns a boolean if anything matched. The match can be retrieved using the get method */
	virtual bool bMatch(std::string str, std::list<std::string> list);
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
}
#endif
