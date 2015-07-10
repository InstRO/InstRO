#ifndef INSTRUMENTOR_BLACKWHITELIST_H__
#define INSTRUMENTOR_BLACKWHITELIST_H__

//#include "../include/common.h"
#include "namematchingselector.h"
#include "compoundselector.h"
//#include "../include/marker.h"

namespace InstRO {

/**
 * \brief Selects or deselects function definitions based on a white- and blacklist.
 *
 *
 * Is based on the BooleanCompoundSelector class.
 * The fileToParse is a simple txt file with a + to be considered as the key for a whitelist entry and
 * - be the one for the blacklist.
 * So as an example, if you want function foo to be on the whitelist and bar on the blacklist the txt
 * file would contain 2 lines.
 * + foo
 * - bar
 * \ingroup Selector
 * \author Jan-Patrick Lehr
 */
class BlackWhiteListSelector : public BooleanCompoundSelector {
 public:
	/** creates a Black- and Whitelist from the file found at the passed location */
	BlackWhiteListSelector(std::string fileToParse, bool preferWhitelist = true);
	/** Visits the AST nodes in pre order */
	void processNode(SgNode* node);
	/** Adds every element of listToAdd to the whitelist */
	void addStringListToWhiteList(std::vector<std::string> listToAdd);
	/** Adds every element of listToAdd to the blacklist */
	void addStringListToBlackList(std::vector<std::string> listToAdd);
	/** Sets if the selector should prefer whitelist over blacklist or vice versa. Default=true */
	void preferWhitelistOverBlacklist(bool preferWhitelist);

 private:
	std::string whiteListSpecification; /**< The suffix for a line, if the specified function name is to be put on the
																				 whitelist */
	std::string blackListSpecification; /**< The suffix for a line, if the specified function name is to be put on the
																				 blacklist */
	int whiteListSpecificationLength; /**< The length of the suffix specifying a function name to be put on the whitelist
																			 */
	int blackListSpecificationLength; /**< The length of the suffix specifying a function name to be put on the blacklist
																			 */
	std::list<std::string> whiteList; /**< The actual whitelist */
	std::list<std::string> blackList; /**< The actual blacklist */
	bool preferWhitelist;							/**< If elements on the whitelist should be preferred over the blacklist */

	NameMatchingSelector whiteListMatcher; /** Namematching Selector instance for the whitelist */
	NameMatchingSelector blackListMatcher; /** Namematching Selector instance for the blacklist */

	/** Adds string str to the whitelist */
	void addStringToWhiteList(std::string str);

	/** Adds string str to the blacklist */
	void addStringToBlackList(std::string str);

	/** Read the inputfile and creates white and blacklist */
	void parseFile(std::string fileToParse);
	/** To avoid redundant code, this function takes a line from the input file, truncates it and adds it to the given
	 * list */
	void qualifyName(std::string& inputLine);

	/** Overwritten from Selector base class to attach black-whitelist specific markers. */
	SelectionASTMarker* createSelectionMarker();
};
}
#endif	// INSTRUMENTOR_BLACKWHITELIST_H__
