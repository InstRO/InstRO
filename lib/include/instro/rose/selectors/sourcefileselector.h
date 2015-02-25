#ifndef SOURCEFILESELECTOR_H
#define SOURCEFILESELECTOR_H

#include "../common.h"
#include <boost/unordered_set.hpp>
#include <boost/algorithm/string.hpp>

namespace InstRO {

/**
 * \brief Receives list of source files that are part of current project. Selects corresponding function definitions.
 * \author Roman Neß
 */
class SourceFileSelector : public OutOfOrderSelector {
public:
	SourceFileSelector(SgProject* project, std::string paths);
	/* is called on every node, especially on every function definition */
	void visit(SgNode* node);


private:
	/* absolute file paths of whitelisted files */
	boost::unordered_set<std::string> pathWhitelist;
	/* absolute file paths of all sources used in the current project */
	boost::unordered_set<std::string> projectSources;

	/* parse given file paths into the whitelist */
	void parsePaths(std::string paths);
	/* add the absolute path of a path to the whitelist */
	void addToWhitelist(std::string path);
	/* determine if a path is whitelisted */
	bool isWhitelisted(std::string path);

};

}

#endif // SOURCEFILESELECTOR_H
