#include <instro/selectors/sourcefileselector.h>

using namespace InstRO;

/* XXX JPL: I think this functionality is completely implemented in the file origin selector (common.cpp)
 * 		So it's a great example why we should refactor some parts of InstRO and update its documentation.
 */

#define VERBOSE 1

/**
	* \brief
	* \param project The project to be traversed
	* \param path Multiple source file paths divided by "," or ":"
	*/
SourceFileSelector::SourceFileSelector(SgProject* project, std::string paths) {
	SgFilePtrList projectFiles = project->get_fileList();

	// RN: XXX this will only find source files, no header files
	for (SgFilePtrList::iterator it = projectFiles.begin(); it != projectFiles.end(); it++) {
		projectSources.insert((*it)->get_sourceFileNameWithPath());
	}

	parsePaths(paths);
}

void SourceFileSelector::visit(SgNode* node) {
	SgFunctionDefinition* funcDef = isSgFunctionDefinition(node);

	if (funcDef) {
		// RN: i guess this will always be the path to the defining declaration;
		std::string fileName = funcDef->get_file_info()->get_filenameString();

		// select if on whitelist
		if (isWhitelisted(fileName)) {
			this->select(funcDef);
		}
	}
}

/**
	* \brief Parses and adds multiple file paths to whitelist
	* \param filePath Multiple relative file paths divided by "," or ":"
	*/
void SourceFileSelector::parsePaths(std::string paths) {
	std::vector<std::string> vecPath;

	boost::algorithm::erase_all(paths, " ");	// erase whitespace
	boost::split(vecPath, paths, boost::is_any_of(",:"));

	for (std::vector<std::string>::iterator it = vecPath.begin(); it != vecPath.end(); it++) {
		addToWhitelist(*it);
	}
}

/**
	*	\brief
	* \param path A potentially relative file path
	*/
void SourceFileSelector::addToWhitelist(std::string path) {
	// get absolute unique filename
	char* absPathBuff = realpath(path.c_str(), 0);	// XXX is this safe?
	if (absPathBuff) {
		std::string absPath(absPathBuff);

		// check whether the new path is actually part of the project
		if (projectSources.find(absPath) != projectSources.end()) {
			pathWhitelist.insert(absPath);

#ifdef VERBOSE
			std::cout << "Added to whitelist: \t" << absPath << std::endl;
#endif
		} else {
			std::cout << "No source of project: \t " << absPath << std::endl;
		}

	} else {
		std::cout << "No such path: \t" << path << std::endl;
	}
}

/**
	*	\brief Returns true iff the given absolute file path is whitelisted
	* \param filePath An absolute file path
	*/
bool SourceFileSelector::isWhitelisted(std::string filePath) {
	return pathWhitelist.find(filePath) != pathWhitelist.end();
}
