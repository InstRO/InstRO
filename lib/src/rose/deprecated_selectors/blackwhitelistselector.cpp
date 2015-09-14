#include <fstream>
#include <string>
#include <instro/selectors/blackwhitelistselector.h>

#define VERBOSE 0

using namespace InstRO;

/* Public functions */
/**
 * \brief Constructor, parses the given file and adds its recognized lines to the internal Black-/Whitelist
 * \param fileToParse The file to parse. Every line is parsed and added to the internal black- or white-list, regarding
 * its prefix
 */
BlackWhiteListSelector::BlackWhiteListSelector(std::string fileToParse, bool preferWhitelist) {
	this->preferWhitelist = preferWhitelist;
	whiteListMatcher = NameMatchingSelector(false);
	blackListMatcher = NameMatchingSelector(false);
	/* JPL: Moved all the code in parseFile to overcome code-redundancy */
	parseFile(fileToParse);
	/* This instance creation is just for debugging purposes */
	WildcardedStringMatcher* wlMatcher = new WildcardedStringMatcher();
#ifdef DEBUG
	wlMatcher->setVerbose(true);
#endif
	whiteListMatcher.init(IN_NextOuterFunctionDefinition, wlMatcher, &whiteList);
	blackListMatcher.init(IN_NextOuterFunctionDefinition, new WildcardedStringMatcher(), &blackList);

	subSelectors.push_back(&whiteListMatcher);
	subSelectors.push_back(&blackListMatcher);
	invSelectors.push_back(false);
	invSelectors.push_back(true);
#ifdef DEBUG
	std::cout << "construction of blackwhitelist selector done." << std::endl;
	std::cout << "SubSelectors: " << whiteListMatcher.getId() << " & " << blackListMatcher.getId() << std::endl;
#endif
}

void BlackWhiteListSelector::parseFile(std::string fileToParse) {
	/* Define beginning of specification-lines to make code more maintainable */
	whiteListSpecification = "+ ";
	blackListSpecification = "- ";
	whiteListSpecificationLength = whiteListSpecification.length();
	blackListSpecificationLength = blackListSpecification.length();

	/* Open an input stream from the given file */
	std::ifstream inputFunction(fileToParse.c_str(), std::ifstream::in);

	/* Declare a new string to hold the current line so it does not have to be redefined in every loop iteration */
	std::string currentLine;

	/* Parse whole file to the end */
	while (!inputFunction.eof()) {
		/* Read one line from the fileToParse and store it in currentLine */
		getline(inputFunction, currentLine);
		if (currentLine.compare(0, whiteListSpecificationLength, whiteListSpecification) == 0) {
			currentLine.erase(0, whiteListSpecificationLength);	// cut off '+ '
																													 /* Add Function to Whitelist */
			addStringToWhiteList(currentLine);
		} else if (currentLine.compare(0, blackListSpecificationLength, blackListSpecification) == 0) {
			currentLine.erase(0, whiteListSpecificationLength);	// cut off '- '
																													 /* Add Function to Blacklist */
			addStringToBlackList(currentLine);
		} else {
			/* If the line does start with neither '+ ' nor '- ', discard it */
		}
	}

	/* Close the file in order to leave cleanly */
	inputFunction.close();
}

/* Overridden from BooleanCompoundSelector */
/**
 * This function gets called for every node in the AST. If the NameMatchingSelector for black and whitelist left their
 * markers, the logical formulation will be evaluated with the following semantics:
 * Semantics (W: Whitelist, B: Blacklist, s: select node):
 * |  | B|-B|
 * | W| X| s|
 * |-W|-s| X|
 * Where X can be: prefer Blacklist or prefer Whitelist.
 *
 * RN: basically..
 * prefer whiteList: W || -B
 * prefer blackList: W && -B
 *
 * \brief Visiting function, gets called for every node and decides whether or not to instrument it.
 * \param node The currently visited node.
 */
void BlackWhiteListSelector::processNode(SgNode* node) {
	if (node == NULL) {
		std::cerr << "BlackWhiteListSelector::processNode :: node was null" << std::endl;
	}
	// Evaluate at each node if there are ast attributes from the whitelist or the blacklist
	ASTAttributeContainer* container = ASTAttributeContainer::hasASTAttributeContainer(node);
	if (container == NULL) {
		return;
	}

	/* Do the actual calculation whether node should be selected or not. */
	bool isSelected;
	if (this->preferWhitelist) {
		// W || -B
		isSelected = evalLogicalOr(container);
	} else {
		// W && -B
		isSelected = evalLogicalAnd(container);
	}

	if (isSelected) {
#if VERBOSE
		std::cout << "BW selects: " << SageInterface::get_name(node) << std::endl;
#endif
		select(node);
	}
}

void BlackWhiteListSelector::addStringListToWhiteList(std::vector<std::string> listToAdd) {
	std::vector<std::string>::iterator i;
	for (i = listToAdd.begin(); i != listToAdd.end(); i++) {
		addStringToWhiteList(*i);
	}
}

void BlackWhiteListSelector::addStringToWhiteList(std::string toAdd) {
	qualifyName(toAdd);
	whiteList.push_back(toAdd);

#if VERBOSE
	std::cout << "Added to WL: " << toAdd << std::endl;
#endif
}

void BlackWhiteListSelector::addStringListToBlackList(std::vector<std::string> listToAdd) {
	std::vector<std::string>::iterator i;
	for (i = listToAdd.begin(); i != listToAdd.end(); i++) {
		addStringToBlackList(*i);
	}
}

void BlackWhiteListSelector::addStringToBlackList(std::string toAdd) {
	qualifyName(toAdd);
	blackList.push_back(toAdd);

#if VERBOSE
	std::cout << "Added to BL: " << toAdd << std::endl;
#endif
}

/**
 * RN: do we still need the possibility to change this attribute after construction?
 */
void BlackWhiteListSelector::preferWhitelistOverBlacklist(bool preferWhitelist) {
	this->preferWhitelist = preferWhitelist;
}

SelectionASTMarker* BlackWhiteListSelector::createSelectionMarker() {
	BWSelectionASTMarker* marker = new BWSelectionASTMarker(this);

	// Do we have to do something more here, like adding the decision base to the marker?
	return marker;
}

/**
 * \brief Qualifies a name by prepending '::' if necessary
 * \param inputLine The method name to be qualified
 */
void BlackWhiteListSelector::qualifyName(std::string& inputLine) {
	/* Check if the given name is a qualified function name, i.e. it begins with "::" */
	if (inputLine.compare(0, 2, "::") != 0) {
		/* Given function name is not qualified - prepend "::" to it */
		inputLine.insert(0, "::");
	} else {
		/* Do nothing - function name is already qualified */
	}
}
