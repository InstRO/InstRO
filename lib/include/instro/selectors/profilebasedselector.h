#ifndef PROFILEBASEDSELECTOR_H
#define PROFILEBASEDSELECTOR_H

#include "../common.h"

#include <boost/unordered_map.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace InstRO {

struct Profile {
	float calls;
	float excl;
	float incl;
};

/**
 * \brief Receives a file containing profiling information. Selects functions with enough runtime.
 * \author Roman Neß
 */
class ProfileBasedSelector : public OutOfOrderSelector {
public:
	ProfileBasedSelector(SgProject* project, std::string path, float thresholdMillies);

	/** is called on every node, especially on every function definition */
	void visit(SgNode* node);


private:
	/** The threshold in milliseconds from which on methods will be selected */
	float thresholdMillies;
	/** scale of profile's runtime information to millies */
	int timeScale;
	/** true if a valid timeScale was parsed */
	bool timeScaleParsed;

	boost::unordered_map<std::string, Profile> profileInformation;

//	/** exclusive runtime (in milli seconds) per call for all known functions */
//	boost::unordered_map<std::string, float> exclRuntimesPerCall;
//	/** inclusive runtime (in milli seconds) per call for all known functions */
//	boost::unordered_map<std::string, float> inclRuntimesPerCall;
//	/** number of calls to all known functions */
//	boost::unordered_map<std::string, float> numberOfCalls;


	/** parse given file paths into the whitelist */
	void parseProfileFromFile(std::string paths);
	/** determine whether the profile is in ms, s, etc. */
	bool parseTimeScale(std::string s);

	/** determine whether a method has enough runtime to be selected */
	bool hasEnoughRuntime(std::string funcName);

	/** dump the 3 hashmaps with profile information in a meaningful way */
	void dumpProfileInformation();
};

}

#endif // PROFILEBASEDSELECTOR_H
