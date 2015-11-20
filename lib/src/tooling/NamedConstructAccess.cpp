#include <memory>		 // We need shared pointers
#include <list>			 // We use List in the GrammarInterface
#include <iostream>	// We use Cout ..
#include "instro/core/ConstructSet.h"
#include "instro/tooling/NamedConstructAccess.h"

#define INSTRO_LOG_LEVEL WARN
#include "instro/utility/Logger.h"


namespace InstRO {
namespace Tooling {
namespace NamedConstructAccess {


bool WildcardedStringMatcher::bMatch(std::string candidate, std::string pattern) {
	logIt(DEBUG) << "\nMatcher started for candidate: \"" << candidate << "\" pattern: \"" << pattern << "\"" << std::endl;

//	matchingStringList.clear();

	int i = 0;					// index in the wild carded string
	int j = 0;					// index in the string
	int lastWild = -1;	// index of last found wild card
	bool partialMatch = false;
	/*
	 * This one holds the index where the search for a match should be restarted after a wildcard was found
	 * but it could not succeed with a match until the end of the string.
	 */
	int lastAfterWild = 0;
	while (j < candidate.length()) {
		logIt(DEBUG) << "i: " << i << "\tj: " << j << std::endl;

		if (!(i < pattern.length())) {
			if (lastWild >= 0) {
				i = lastWild;
				j++;
			} else {
				break;
			}
		}

		if (!(j < candidate.length())) {
			if (lastWild >= 0) {
				j = lastAfterWild + 1;
			} else {
				break;
			}
		}

		if (pattern[i] == WILDCARDCHAR) {	// if wildcard
			logIt(DEBUG) << "pattern[i] == Wildcardchar" << std::endl;
			lastWild = i;				// save index
			lastAfterWild = j;	// save initial position to search in string
			i++;								// increment to next char in wild-string
			partialMatch = true;
		} else if (pattern[i] == candidate[j]) {	// if two chars match
			logIt(DEBUG) << "candidate[j] == pattern[i]" << std::endl;
			i++;	// increment in wild-string
			j++;	// increment in string
			partialMatch = true;
		} else if (lastWild >= 0 && pattern[lastWild] == WILDCARDCHAR) {	// if wild-string really contains wildcard
			logIt(DEBUG) << "Reset case" << std::endl;
			j = lastAfterWild + 1;	// restart search at the saved position in string
			i = lastWild;						// restart at the last wildcard character
			partialMatch = false;
		} else {
			logIt(DEBUG) << "Other case" << std::endl;
			partialMatch = false;
			break;
		}
	}

	if (i >= pattern.length() - 1 && pattern[pattern.length() - 1] == WILDCARDCHAR) {

		logIt(INFO) << "\nFirst case " << std::endl;
		logIt(DEBUG) << pattern << " matches: " << candidate << "\nPartial Match: " << partialMatch << std::endl;

		return true;
	} else if (j == candidate.length() && i == pattern.length()) {

		logIt(INFO) << "\nSecond Case" << std::endl;
		logIt(DEBUG) << pattern << " matches: " << candidate << "\nPartial Match: " << partialMatch << std::endl;

		return true;
	} else {
		logIt(INFO) << "No matching.\nPartial Match: " << partialMatch << std::endl;
		return false;
	}
}

bool WildcardedStringMatcher::isMatch(std::string candidate, std::string pattern) {
	return bMatch(candidate, pattern);
}

bool WildcardedStringMatcher::isMatch(std::string candidate) {
	for(auto p : patterns){
		if(bMatch(candidate, p)){
			return true;
		}
	}
	return false;
}

}
}
}
