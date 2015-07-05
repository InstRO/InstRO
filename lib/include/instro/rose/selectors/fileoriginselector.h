#ifndef FILEORIGINSELECTOR_H
#define FILEORIGINSELECTOR_H

#include "../common.h"

namespace InstRO {

/**
 * \brief FileOriginSelector: Selects files by their origin.
 *
 *
 * It is possible to use either whole paths or single files.
 * For files originating from the commandline there is a switch for determining explicitly to whether mark them or not.
 * In difference to the SourceFileSelector, with this selector it is possible to include or exclude complete directories
 * from selection phase.
 *
 * \ingroup Selector
 * \author InstRO
 */
class FileOriginSelector : public OutOfOrderSelector {
 public:
	/* Using the constructor files originating from the commandline are excluded from selection process */
	FileOriginSelector(SgProject* project);
	/** If markingSwitch is set to false, files which are specified as arguments on the command line are not selected */
	void markComesFromCommandLine(bool markingSwitch);
	void visit(SgNode* node);

	void addFolderToList(std::string);
	void addFileToList(std::string);
	void addFileToList(SgFile*);

 private:
	std::list<std::string> folderList;
	std::list<SgFile*> fileList;
	bool markingSwitch;

	SgProject* project;

	bool comesFromCommandLine(SgNode* codeToLocate);
	bool comesFromFile(SgNode* codeToLocate, SgFile* candidateFile);
};
}

#endif
