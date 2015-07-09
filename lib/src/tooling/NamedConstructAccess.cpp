#include <memory>		 // We need shared pointers
#include <list>			 // We use List in the GrammarInterface
#include <iostream>	// We use Cout ..
#include "instro/core/ConstructSet.h"
#include "instro/tooling/NamedConstructAccess.h"

namespace InstRO {
namespace Tooling {
namespace NamedConstructAccess {

/*
* Method to match wildcarded strings.
* The '#' character is used as wildcard and can be posed anywhere. This means
* a wildcarded string can be in the form of
* foo, #foo, foo#, f#o
* and any mixtures. The wildcard stands for an arbitrary amount of characters,
* therefore f#o would match foo as well as fooooo or fasdfo.
*/
std::vector<int> WildcardedStringMatcher::match(std::string str, std::list<std::string> list) {
	/* At the beginning we clear the matching list */
	this->matchingList.clear();
	this->matchingStringList.clear();

	if (this->verbose) {
		std::cout << "\n===Matcher verbose mode===" << std::endl;
		std::cout << "WildcardedStringMatcher::match called with arguments: " << std::endl;
		std::cout << "pattern: " << str << std::endl;
		std::cout << "list: ";
		std::list<std::string>::iterator iter;
		for (iter = list.begin(); iter != list.end(); iter++) {
			std::cout << *iter << ", ";
		}
		// std::cout << "\n===Matcher verbose end===" << std::endl;
	}

	std::vector<int> matches = std::vector<int>(0);

	std::list<std::string>::iterator listIter;
	int position = 0;
	for (listIter = list.begin(); listIter != list.end(); listIter++) {
		std::string listItem = *listIter;

		unsigned int i = 0;	// index in the wild carded string
		unsigned int j = 0;	// index in the string
		int lastWild = -1;	 // index of last found wild card
		bool partialMatch = false;
		/* This one holds the index where the search for a match should be restarted after a wildcard was found
		* but it could not succeed with a match until the end of the string.
		*/
		int lastAfterWild = 0;
		//		int iterationCounter = 0;
		while (i < listItem.length() && j < str.length() /* && iterationCounter < 10*/) {
			//			std::cout << "i: " << i << "\tj: " << j << std::endl; iterationCounter++;
			if (listItem[i] == WILDCARDCHAR) {	// if wildcard
				lastWild = i;											// save index
				lastAfterWild = j;								// save initial position to search in string
				i++;															// increment to next char in wild-string
				partialMatch = true;
			} else if (listItem[i] == str[j]) {	// if two chars match
				i++;															 // increment in wild-string
				j++;															 // increment in string
				partialMatch = true;
			} else if (lastWild >= 0 && listItem[lastWild] == WILDCARDCHAR) {	// if wild-string really contains wildcard
				j = lastAfterWild + 1;	// restart search at the saved position in string
				i = lastWild;						// restart at the last wildcard character
				partialMatch = false;
			} else {
				// If none of these cases matched, break as there is no match
				//				std::cout << "Break" << __LINE__ << std::endl;
				partialMatch = false;
				break;
			}
		}
#if 0
					std::cout << "i: " << i << "\tj: " << j << std::endl;
					std::cout << "listItem:" << listItem.length() << " with value: " << listItem[i] << std::endl;
					std::cout << "pattern: " << pattern.length() << " with value: " << pattern[j] << std::endl;
#endif
		// TODO 2013-10-08 JP: Check if the whole matching can be reduced by using bool partialMatch.
		// TODO 2013-10-08 JP: Have a bunch of tests to proof correctness.
		if (i >= listItem.length() - 1 && listItem[listItem.length() - 1] == WILDCARDCHAR) {
			// at the end of the wild-carded string and last char is a wild-card
			if (this->verbose) {
				std::cout << "\nFirst case matches: " << std::endl;
				std::cout << "Matcher verbose: " << listItem << " matches: " << str << "\nPartial Match: " << partialMatch
									<< std::endl;
			}
			//			this->matchingList.push_back(listItem);
			//			this->matchingList.push_back(position); // XXX could be a map
			matches.push_back(position);
			matchingStringList.push_back(listItem);
		} else if (j == str.length() && i == listItem.length() - 1) {
			// why exactly is that a match?
			if (this->verbose) {
				std::cout << "\nSecond Case matches: " << listItem << " matches: " << str << std::endl;
				std::cout << "Matcher verbose: " << listItem << " matches: " << str << "\nPartial Match: " << partialMatch
									<< std::endl;
			}
			//			this->matchingList.push_back(listItem);
			//			this->matchingList.push_back(position);
			matches.push_back(position);
			matchingStringList.push_back(listItem);
		} else if (j == str.length() && i == listItem.length()) {
			// exact match
			if (this->verbose) {
				std::cout << "\nThird case matches: " << std::endl;
				std::cout << "Matcher verbose: " << listItem << " matches: " << str << "\nPartial Match: " << partialMatch
									<< std::endl;
			}
			//			this->matchingList.push_back(listItem);
			//			this->matchingList.push_back(position);
			matches.push_back(position);
			matchingStringList.push_back(listItem);
		} else {
			// No Match
			if (this->verbose) {
				std::cout << "No matching."
									<< "\nPartial Match: " << partialMatch << std::endl;
			}
		}
		position++;
		// end of loop
	}
//	std::cout << "right before return matchingList has size: " << matchingList.size() << std::endl;
/*	if(matchingList.size() > 0){
std::cout << "Matching item: " << matchingList.front() << " with value: " << list.front() << std::endl;
}
*/
#if 0		// --- 2013-10-08 JP: I guess this code can be erased
				// copy the locally created content
				matchingList.assign(matches.begin(), matches.end());

				// XXX JP: Change this!
				std::vector<int>::iterator iter;
				std::list<std::string>::iterator lIter;
				//    std::cout << "Number of Matches: " << matchingList.size() << std::endl;
				for (iter = matchingList.begin(); iter != matchingList.end(); iter++) {
					int k = (*iter);
					lIter = list.begin();
					for (int j = 0; j < k; j++) {
						lIter++;
					}
					matchingStringList.push_back((*lIter));
				}
#endif	// --- until here
#ifdef DEBUG
	std::vector<std::string>::iterator iter;
	std::cout << "Matcher going to print matching patterns." << std::endl;
	for (iter = matchingStringList.begin(); iter != matchingStringList.end(); iter++) {
		std::cout << *iter << std::endl;
	}
#endif

	// return the new copies
	return matchingList;

#if 0	// -- 2013-10-08 JP: I don't know why this is here
				// if this goes wrong, simply use this statement instead of the two lines above
				//return matches;
#endif
}

bool WildcardedStringMatcher::bMatch(std::string str, std::list<std::string> list) {
	/* At the beginning we clear the matching list */
	this->matchingList.clear();
	this->matchingStringList.clear();

	if (this->verbose) {
		std::cout << "\n===Matcher verbose mode===" << std::endl;
		std::cout << "WildcardedStringMatcher::match called with arguments: " << std::endl;
		std::cout << "pattern: " << str << std::endl;
		std::cout << "list: ";
		std::list<std::string>::iterator iter;
		for (iter = list.begin(); iter != list.end(); iter++) {
			std::cout << *iter << ", ";
		}
		// std::cout << "\n===Matcher verbose end===" << std::endl;
	}

	std::vector<int> matches = std::vector<int>(0);

	std::list<std::string>::iterator listIter;
	int position = 0;
	for (listIter = list.begin(); listIter != list.end(); listIter++) {
		std::string listItem = *listIter;

		int i = 0;					// index in the wild carded string
		int j = 0;					// index in the string
		int lastWild = -1;	// index of last found wild card
		bool partialMatch = false;
		/* This one holds the index where the search for a match should be restarted after a wildcard was found
		* but it could not succeed with a match until the end of the string.
		*/
		int lastAfterWild = 0;
		//		int iterationCounter = 0;
		while (i < listItem.length() && j < str.length() /* && iterationCounter < 10*/) {
			//			std::cout << "i: " << i << "\tj: " << j << std::endl; iterationCounter++;
			if (listItem[i] == WILDCARDCHAR) {	// if wildcard
				lastWild = i;											// save index
				lastAfterWild = j;								// save initial position to search in string
				i++;															// increment to next char in wild-string
				partialMatch = true;
			} else if (listItem[i] == str[j]) {	// if two chars match
				i++;															 // increment in wild-string
				j++;															 // increment in string
				partialMatch = true;
			} else if (lastWild >= 0 && listItem[lastWild] == WILDCARDCHAR) {	// if wild-string really contains wildcard
				j = lastAfterWild + 1;	// restart search at the saved position in string
				i = lastWild;						// restart at the last wildcard character
				partialMatch = false;
			} else {
				// If none of these cases matched, break as there is no match
				//				std::cout << "Break" << __LINE__ << std::endl;
				partialMatch = false;
				break;
			}
		}
		// TODO 2013-10-08 JP: Check if the whole matching can be reduced by using bool partialMatch.
		// TODO 2013-10-08 JP: Have a bunch of tests to proof correctness.
		if (i >= listItem.length() - 1 && listItem[listItem.length() - 1] == WILDCARDCHAR) {
			// at the end of the wild-carded string and last char is a wild-card
			if (this->verbose) {
				std::cout << "\nFirst case matches: " << std::endl;
				std::cout << "Matcher verbose: " << listItem << " matches: " << str << "\nPartial Match: " << partialMatch
									<< std::endl;
			}
			//			this->matchingList.push_back(listItem);
			//			this->matchingList.push_back(position); // XXX could be a map
			matches.push_back(position);
			matchingStringList.push_back(listItem);
		} else if (j == str.length() && i == listItem.length()) {
			// why exactly is that a match?
			if (this->verbose) {
				std::cout << "\nSecond Case matches: " << listItem << " matches: " << str << std::endl;
				std::cout << "Matcher verbose: " << listItem << " matches: " << str << "\nPartial Match: " << partialMatch
									<< std::endl;
			}
			//			this->matchingList.push_back(listItem);
			//			this->matchingList.push_back(position);
			matches.push_back(position);
			matchingStringList.push_back(listItem);
		} else if (j == str.length() && i == listItem.length()) {
			// exact match
			if (this->verbose) {
				std::cout << "\nThird case matches: " << std::endl;
				std::cout << "Matcher verbose: " << listItem << " matches: " << str << "\nPartial Match: " << partialMatch
									<< std::endl;
			}
			//			this->matchingList.push_back(listItem);
			//			this->matchingList.push_back(position);
			matches.push_back(position);
			matchingStringList.push_back(listItem);
		} else {
			// No Match
			if (this->verbose) {
				std::cout << "No matching."
									<< "\nPartial Match: " << partialMatch << std::endl;
			}
		}
		position++;
		// end of loop
	}

	// return the new copies
	return (matchingStringList.size() > 0) ? true : false;
}

std::vector<std::vector<int> > WildcardedStringMatcher::match(std::vector<std::string> strList,
																															std::list<std::string> list) {
	// XXX 2013-10-08 JP: Change this to a more sophisticated implementation
	std::vector<std::vector<int> > returnVec;
	std::vector<int> localVec;
	std::vector<std::string>::iterator i;
	for (i = strList.begin(); i != strList.end(); i++) {
		localVec = match((*i), list);
		returnVec.push_back(localVec);
	}
	return returnVec;
}
bool WildcardedStringMatcher::isMatch(std::string input) {
	// 1rst arg is the wild-card list, 2nd arg is the list of strings to match against
	std::list<std::string> aList;
	aList.push_front(input);
	bool haveMatch = false;
	for (auto expr : wildcardStrings) {
		bool tmp = bMatch(expr, aList);
		if (this->verbose)
			std::cout << "Matching with expr: " << expr << " to " << input << " result " << tmp << std::endl;
		if (tmp)
			return true;
		//		haveMatch|= bMatch(expr,aList);
	}
	return haveMatch;
	return bMatch(input, wildcardStrings);
}

std::vector<int> WildcardedStringMatcher::getMatchIds() { return matchingList; }

void WildcardedStringMatcher::setVerbose(bool verbose) { this->verbose = verbose; }

std::vector<std::string> WildcardedStringMatcher::getMatchingStringList() { return this->matchingStringList; }
}
}
}
