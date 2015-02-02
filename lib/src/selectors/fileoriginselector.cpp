#include "instro/selectors/fileoriginselector.h"

#define VERBOSE 5

/* FO_Selector */
FileOriginSelector::FileOriginSelector(SgProject* project) :
		project(project) {
	this->markingSwitch = false;
}

void FileOriginSelector::markComesFromCommandLine(bool markingSwitch) {
	this->markingSwitch = markingSwitch;
}

void FileOriginSelector::visit(SgNode* node) {
	// Immediately return if we do not have a located node. Only these have a file to be located in.
	if (!isSgLocatedNode(node)) {
		return;
	}

	SgLocatedNode* localNode = isSgLocatedNode(node);
	// 2013-07-05 JPL: Since instantiated templates are marked as compiler generated and therefore do not include
	//                          any file name, we have to get the template declaration it was generated from.
	// XXX: If the template specialization is given in a file and is not compiler generated get_templateDeclaration seg faults.
	//          in this case the file_info object is directly attached to the template instantiation.
	if (isSgFunctionDefinition(node))
		localNode = isSgLocatedNode(node->get_parent());

	if (isSgTemplateInstantiationFunctionDecl(localNode)) {
#if VERBOSE > 4
		std::cout << "FileOriginSelector::" << __FUNCTION__ << "  node: " << localNode << " : " << isSgTemplateInstantiationFunctionDecl(localNode)->get_name() << "\n" << localNode->class_name() << "\nisCompilerGenerated:" << localNode->get_file_info()->isCompilerGenerated() << std::endl;
#endif
		if (localNode->get_file_info()->isCompilerGenerated())
			localNode = isSgTemplateInstantiationFunctionDecl(localNode)->get_templateDeclaration();
#if VERBOSE > 4
	std::cout << "FileOriginSelector::"<< __FUNCTION__ << "  LocalNode's File Info: \n" << localNode->get_file_info()->get_filenameString() << std::endl;
#endif
	} else if (isSgTemplateInstantiationMemberFunctionDecl(localNode)) {
#if VERBOSE > 4
	std::cout << "FileOriginSelector::"<< __FUNCTION__ << "  node: " << localNode << " : " << isSgTemplateInstantiationMemberFunctionDecl(localNode)->get_name() << "\n"<< localNode->class_name() << " isCompilerGenerated:" << localNode->get_file_info()->isCompilerGenerated() << std::endl;
	std::cout << localNode->unparseToString() << std::endl;
#endif
		if (localNode->get_file_info()->isCompilerGenerated()) {
			localNode = isSgTemplateInstantiationMemberFunctionDecl(localNode)->get_templateDeclaration();
#if VERBOSE > 4
			std::cout << "FileOriginSelector: localNode->class_name(): " << localNode->class_name() << std::endl;
#endif
			// This is to solve the problem if an template instantiataion was made fr another instantiation.
			// XXX Check whether this can happen for template functions instead of template member functions as well.
			while (isSgLocatedNode(localNode) != NULL && isSgLocatedNode(localNode)->get_file_info()->isCompilerGenerated())
				if (isSgTemplateInstantiationDecl(localNode))
					localNode = isSgTemplateInstantiationDecl(localNode)->get_templateDeclaration();
				else
					localNode = isSgLocatedNode(localNode->get_parent());
		}
//      std::cout << "LocalNode's File Info: \n" << localNode->get_file_info()->get_filenameString() << std::endl;
	}
	// Appearently we do have a located node - so we need to check if the file it is from is to be instrumented
//    std::string currentFilename = isSgLocatedNode(node)->get_file_info()->get_filenameString();
	// 2013-07-05 JPL: Changed this to account for the template things.
	std::string currentFilename = isSgLocatedNode(localNode)->get_file_info()->get_filenameString();

	// Check if the file the function is in has been specified on the commandline
	if (markingSwitch && comesFromCommandLine(node)) {
		this->select(node);
#if VERBOSE > 2
		std::cout << "Marking node " << node  << " because of coming from command line." << std::endl;
		if(isSgFunctionDeclaration(node))
			std::cout << "Function name: " << isSgFunctionDeclaration(node)->get_name() << std::endl;
#endif
		return;
	}

	// Check if the given file is on the folder-whitelist
	for (std::list<std::string>::iterator i = folderList.begin(); i != folderList.end(); i++) {
		// Check for each entry on the folderwhitelist, if the current folder is shorter than the one of the given node
		if ((*i).length() < currentFilename.length() && (*i).compare(0, (*i).length(), currentFilename, 0, (*i).length()) == 0) {
			// The folder the file is in is specified on the whitelist - so we may instrument it and return
			this->select(node);
#if VERBOSE > 2
			std::cout << "Marking node " << node << " because of file origin matching." << std::endl;
			if(isSgFunctionDeclaration(node))
				std::cout << "Function name: " << isSgFunctionDeclaration(node)->get_name() << " in file: " << isSgLocatedNode(node)->get_file_info()->get_filenameString() << std::endl;
#endif
			return;
		}
	}
	// XXX 2013-07-05 JP: This hack enables the FO Selector to mark nodes which are Compiler generated and
	//                                  use the NULL_FILE p_fileflag to indicate. ROSE doesn't seem to consistently handle its beahvior here.
	if (currentFilename.compare("NULL_FILE") == 0) {
		this->select(node);
#if VERBOSE > 2
		std::cout << "Marking node " << node << "of node->class_name(): " << node->class_name() << " because of NULL_FILE tag." << std::endl;
		if(isSgFunctionDeclaration(node))
			std::cout << "Function name: " << isSgFunctionDeclaration(node)->get_name() << std::endl;
#endif
		return;
	}
	// Apparently the given file is not on the folder-whitelist
	// So we check if this file is specified explicitely

	// H-AW (22/03/2011): Outdated, since we changed the storage of the whitelist to SgFile* instead of std::string
	// First we have to find the filename, assuming / as delimiter
	//currentFilename.erase(0, currentFilename.find_last_of('/')+1);

	for (std::list<SgFile*>::iterator i = fileList.begin(); i != fileList.end(); i++) {
		if ((*i)->get_file_info()->isSameFile(node->get_file_info())) {
			// This exact filename is specified on the whitelist - there is nothing speaking against it being instrumented
			this->select(node);
#if VERBOSE > 2
			std::cout << "Marking node because of same file" << std::endl;
			if(isSgFunctionDeclaration(node))
				std::cout << "Function name: " << isSgFunctionDeclaration(node)->get_name() << std::endl;
#endif
			return;
		}
	}

	// Since we have not found the node on any whitelist we may not instrument it
	// H-AW: Possible, but would clutter up the ast if there are huge headers being included
	// this->unselect(node);
#if VERBOSE > 2
	if(isSgTemplateInstantiationFunctionDecl(localNode)) {
		std::cout << "in File: " << currentFilename << std::endl;
		std::cout << "node: " << localNode << " isCompilerGenerated:" << localNode->get_file_info()->isCompilerGenerated() << std::endl;
		if(localNode->get_file_info()->isCompilerGenerated()) {
			std::cout << isSgTemplateInstantiationFunctionDecl(localNode)->get_templateDeclaration()->get_name() << std::endl;
			std::cout << isSgTemplateInstantiationFunctionDecl(localNode)->get_templateDeclaration()->class_name() << std::endl;
		}
	} else if(isSgTemplateInstantiationMemberFunctionDecl(localNode)) {
		std::cout << "in File: " << currentFilename << std::endl;
		std::cout << "node: " << localNode << " isCompilerGenerated:" << localNode->get_file_info()->isCompilerGenerated() << std::endl;
		if(localNode->get_file_info()->isCompilerGenerated()) {
			std::cout << isSgTemplateInstantiationMemberFunctionDecl(localNode)->get_templateDeclaration()->get_name() << std::endl;
			std::cout << isSgTemplateInstantiationMemberFunctionDecl(localNode)->get_templateDeclaration()->class_name() << std::endl;
		}
	}
#endif
	return;
}

/**
 * \brief Returns true iff the given node comes from a file specified on the command line for the given project
 * \return true iff the given node comes from a file specified on the command line for the given project
 */
bool FileOriginSelector::comesFromCommandLine(SgNode* codeToLocate) {
	/* Get all files passed on the command line */
	SgFilePtrList filesOnCommandLine = project->get_fileList();

	/* Check for every file if the code originates from there */
	for (SgFilePtrList::iterator j = filesOnCommandLine.begin(); j != filesOnCommandLine.end(); j++) {
		if (comesFromFile(codeToLocate, *j)) {
			return true;
		}
	}

	/* Since we have traversed all files and not yet returned true, the file is not specified on the command line */
	return false;
}

/**
 * \brief Checks if the passed node is located in the given file
 * \param codeToLocate The node whose location is to be checked
 * \param candidateFile The file that the given node is supposed to originate from
 * \return True iff the given node originates from the given file. False if the codeToLocate is not a SgLocatedNode*
 */
bool FileOriginSelector::comesFromFile(SgNode* codeToLocate, SgFile* candidateFile) {
	if (isSgLocatedNode(codeToLocate)) {
		return candidateFile->get_file_info()->isSameFile(codeToLocate->get_file_info());
	}
	return false;
}

void FileOriginSelector::addFileToList(SgFile* file) {
	fileList.push_back(file);
}

void FileOriginSelector::addFileToList(std::string file) {
	std::cerr << "This Function is not supported for the moment." << std::endl;
	SgFile * f = SageBuilder::buildFile(file, file, NULL);
	fileList.push_back(f);
}

void FileOriginSelector::addFolderToList(std::string path) {
	folderList.push_back(path);
}
