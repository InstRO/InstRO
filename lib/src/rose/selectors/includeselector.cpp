#include "instro/selectors/includeselector.h"

/* INC_Selector */
IncludeSelector::IncludeSelector(SgProject* project) : project(project){};

#include <iostream>
void IncludeSelector::traverseAST(SgNode* start) {
	class PreprocessorCollection : public AstSimpleProcessing {
	 public:
		// Constructor
		std::vector<std::string> get_includedFiles() { return this->includedFiles; }

	 private:
		std::vector<std::string> includedFiles;

		void visit(SgNode* node) {
			SgLocatedNode* locatedNode = isSgLocatedNode(node);
			if (locatedNode != 0) {
				AttachedPreprocessingInfoType* comments = locatedNode->getAttachedPreprocessingInfo();
				if (comments != 0) {
					// AttachedPreprocessingInfoType is merely a std::list<PreprocessingInfo*> - more info to be found in
					// rose_attributes_list.h:330
					AttachedPreprocessingInfoType::iterator i;
					for (i = comments->begin(); i != comments->end(); i++) {
						if (isIncludeStatement(*i) && isIncludedByQuotationMarks(*i)) {
							// Add the qualified name to the whitelist
							this->includedFiles.push_back(
									StringUtility::getPathFromFileName(node->get_file_info()->get_filenameString()) + "/" +
									get_includedFile(*i));
							std::cout << "Adding " << get_includedFile(*i) << " to includedFiles" << std::endl;
						}
					}
				}
			}
		}

		bool isIncludeStatement(PreprocessingInfo* preprocInfo) {
			return preprocInfo->getTypeOfDirective() == PreprocessingInfo::CpreprocessorIncludeDeclaration;
		}

		bool isIncludedByQuotationMarks(PreprocessingInfo* preprocInfo) {
			// We assume that we are passed something which is definitely an include-Declaration
			std::string preprocString = preprocInfo->getString();
			//! TODO: Find out why the offset of -2 is needed. Shouldn't it be at most -1?
			return !(preprocString.compare(preprocString.find_last_not_of(" \n"), 1, "\"", 1));
		}

		std::string get_includedFile(PreprocessingInfo* preprocInfo) {
			std::string preprocString = preprocInfo->getString();

			char openingMark, closingMark;

			// Determine which quotation marks to use
			if (isIncludedByQuotationMarks(preprocInfo)) {
				// We can be sure that the filename is included in ""
				openingMark = '"';
				closingMark = '"';
			} else {
				// We can be sure that the filename is included in <>
				openingMark = '<';
				closingMark = '>';
			}

			// Remove everything except the string inbetween the quotation marks
			preprocString.erase(0, preprocString.find_first_of(openingMark) + 1);
			preprocString.erase(preprocString.find_last_of(closingMark), preprocString.length());

			return preprocString;
		}
	} preprocCollection;
	preprocCollection.traverse(project, preorder);
	std::vector<std::string> includedFilenames = preprocCollection.get_includedFiles();

	// For every included file create a new SgSourceFile, set the language to c++ and parse it
	std::vector<std::string> args = project->get_originalCommandLineArgumentList();
	CommandlineProcessing::removeAllFileNamesExcept(args, project->get_sourceFileNameList(), "");

	for (std::vector<std::string>::iterator i = includedFilenames.begin(); i != includedFilenames.end(); i++) {
		// Create a new command line with all original options, but with only the current file
		args.push_back(*i);

		std::cout << "Creating a new sourcefile out of the following commandline\n\t";
		for (std::vector<std::string>::iterator j = args.begin(); j != args.end(); j++)
			std::cout << *j << " ";
		std::cout << std::endl;
		// Create a new sourcefile-node and add it to the project
		SgSourceFile* newfile = new SgSourceFile(args, project);

		// Initialize and parse the new file
		newfile->initializeGlobalScope();
		newfile->set_outputLanguage(SgFile::e_Cxx_output_language);
		newfile->callFrontEnd();

		// Make the suffix of the header known to the frontend
		CommandlineProcessing::addCppSourceFileSuffix(StringUtility::fileNameSuffix(*i));

		std::cout << "Building new project with commandline " << CommandlineProcessing::generateStringFromArgList(args)
							<< std::endl;
		/*
			// Create a new project containing only the current file
			SgProject* header_project = frontend(args);


			// JPL: In addition we have to include every possible transitive dependency on any include file.
			preprocCollection.traverse(header_project, preorder);
			std::vector<std::string> transitiveIncludes = preprocCollection.get_includedFiles();
			args.insert(args.end(), transitiveIncludes.begin(), transitiveIncludes.end());



			// Get the only file in the project
			std::cout << "Files in the newly created project: " << header_project->get_fileList().size() << std::endl;
			SgFile* header_file = *(header_project->get_fileList().begin());
		*/
		// Explicitely add the new file to the project
		project->get_fileList().push_back(newfile);

		// Remove the new file from the command line, so we may reuse it later
		args.pop_back();
	}

	/*
* JPL 7/3/12: This was an attempt to handle included files, especially includes in includes, but this is not stable!
*/
	/*
	SgProject* header_project = NULL;
	SgProject* p = NULL;
	std::vector<std::string> topLevelFilenames = includedFilenames;

	// This is a really bad hack to overcome the problem with the vector reallocation (invalidating all iterators for this
vector)
	topLevelFilenames.reserve(topLevelFilenames.size()*5);
	for(std::vector<std::string>::iterator i = topLevelFilenames.begin(); i != topLevelFilenames.end(); i++){
			int j = 0;
//        JPL: There is the need to include every preceeding include file as well to overcome potential
//            problems with typedefs etc.

			args.push_back(*i);
			CommandlineProcessing::addCppSourceFileSuffix(StringUtility::fileNameSuffix(*i));
			// Need to involve frontend here in order to get a list of all used includes
			if(header_project == NULL){
					header_project = frontend(args);
					preprocCollection.traverse(header_project, preorder);
			} else {
					// Add additional includes to the project to make typedefs visible
					args.insert(args.end()-1, topLevelFilenames.begin(), i);
					p = frontend(args);
					preprocCollection.traverse(p, preorder);
			}
			for(std::vector<std::string>::iterator iter = preprocCollection.get_includedFiles().begin()+1; iter !=
preprocCollection.get_includedFiles().end(); iter++){
					std::cout << "Outer Loop: " << *i << std::endl;
					std::cout << "Value " << *iter << std::endl;
			}
			// only insert include files if it has not been included already
			if(!preprocCollection.get_includedFiles().empty()){
					std::vector<std::string> tmpToInclude;
					int k = 0;
					for(std::vector<std::string>::iterator j = preprocCollection.get_includedFiles().begin()+1; j !=
preprocCollection.get_includedFiles().end(); j++){
							std::cout << "Iteration: " << k << " with vectorsize " << preprocCollection.get_includedFiles().size() <<
std::endl;
							if((*j).compare(*i) == 0){
									//  std::cout << "Comparing: i= " << (*i) << " with j=" << (*j) << " continuing." << std::endl;
									j = preprocCollection.get_includedFiles().erase(j);
							} else {
									//  std::cout << "Comparing: i= " << (*i) << " with j=" << (*j) << " adding." << std::endl;
									tmpToInclude.push_back((*j));
							}
							k++;
					}
					//                  std::cout << "Adding " << tmpToInclude.size() << " elements in vector with capacity: " <<
topLevelFilenames.capacity() << std::endl;

					topLevelFilenames.insert(i, tmpToInclude.begin(), tmpToInclude.end());
					// If p is still NULL there exists only the header project, so there's no need to copy anything
					if(p != NULL){
							for(SgFilePtrList::iterator k = p->get_fileList().begin(); k != p->get_fileList().end(); k++){
									header_project->get_fileList().push_back(*(k));
							}
					}
			}
			args.pop_back();
	}
	// Add all found includes to our main project
	for(SgFilePtrList::iterator i = header_project->get_fileList().begin(); i != header_project->get_fileList().end();
i++){
			std::cout << "Adding file: " << (*i)->get_sourceFileNameWithPath() << " to the main project." << std::endl;
			project->get_fileList().push_back(*(i));
	}

*/
}

inline void IncludeSelector::select(SgNode* node) {}

#if 0
SelectionASTMarker * IncludeSelector::getSelectionMarker(SgNode* node){
  return new SelectionASTMarker();
}

#endif
