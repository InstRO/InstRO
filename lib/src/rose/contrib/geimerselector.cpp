#include <instro/contrib/geimerselector.h>

using namespace InstRO;

GeimerSelector::GeimerSelector(std::string const& specFile)
		: gController(GeimerConfigurationController(specFile.c_str())) {
	this->specFile = specFile;
	// this->gController(specFile.c_str());
}

GeimerSelector::GeimerSelector(const char* specFile) : gController(GeimerConfigurationController(specFile)) {
	this->specFile = std::string(specFile);
	// this->gController(specFile.c_str());
}

// XXX DEPRECATED? NEEDED IN ANY WAY?
GeimerSelector::GeimerSelector(std::vector<std::string> whiteList) : gController(GeimerConfigurationController("")) {
	this->whiteList = whiteList;
	// this->gController("");
}

GeimerSelector::~GeimerSelector() {
	//    delete gParser;
	//    delete gCodeGen;
}

bool GeimerSelector::isRoutineInFileToBeInstrumented(std::string routinename, std::string filename) {
	// std::cout << "filename: " << filename << std::endl;

	// Again some string processing... at least if its a full qualified filename...
	size_t delimiter = filename.rfind("/");
	std::string procFilename = filename.substr(delimiter + 1);	// erase full qualification (I guess the offset is needed)

	return gController.isRoutineInFileToBeInstrumented(routinename, procFilename);
}

void GeimerSelector::traverseAST(SgNode* start) {
	/*
	JPL: This is now encapsulated in the GeimerConfigurationController

	// --- prepare spec AST
	//std::cout << "Parser initialization" << std::endl;
	gParser = new GeimerConfigurationParser(specFile.c_str());
	gParser->parseFile();
	std::cout << "Parsed File. " << std::endl;

	// Generate the "abstract" code
	//std::cout << "Code Generator initialization" << std::endl;
	gCodeGen = new GeimerAbstractCodeGenerator(specFile.c_str(), gParser);
	gCodeGen->generate();
	std::cout << "Code generation done." << std::endl;
*/
	gController.run();	// does file parsing and abstract code generation.
	// prepare a string vector for rose frontend
	std::vector<std::string> aVec;
	aVec.push_back(std::string("g++"));
	//    aVec.push_back(gCodeGen->getGeneratedFilename());
	aVec.push_back(gController.getGeneratedFilename());

	this->specProj = frontend(aVec);

	// start the actual processing of nodes
	startNodeProcessing(start);
}

void GeimerSelector::startNodeProcessing(SgNode* startNode) {
	/* JPL: Moved the code from traverseAST to startNodeProcessing to separate the concerns of preparation and actual
	 * processing more clearly */
	// prepare NameMatchingSelector
	WildcardedStringMatcher* matcherInstance = new WildcardedStringMatcher();
	nameMatchingSelector = new NameMatchingSelector(false);
	// Beacuse of the signature of NameMatchingSelector.init
	/*
	JPL: Encapsulated in GeimerConfigurationController
	whiteList = gParser->getAllRoutineNames();
	std::list<std::string> listToMatch;
	listToMatch.assign(this->whiteList.begin(), this->whiteList.end());
*/
	std::list<std::string> listToMatch = gController.getWhiteList();
	nameMatchingSelector->init(IN_Immediate, matcherInstance, &listToMatch);
	// run NameMatchingSelector
	nameMatchingSelector->traverseAST(startNode);

	// We add a GeimerSelector ASTMarker to the first GlobalScope for the includes
	SgGlobal* gScope = SageInterface::getFirstGlobalScope(SageInterface::getProject());

	//   std::vector<FileParserGlobalPragma*> includePragmas = gParser->getIncludePragmas();

	std::vector<FileParserGlobalPragma*> includePragmas = gController.getIncludePragmas();

	std::vector<FileParserGlobalPragma*>::iterator j;
	for (j = includePragmas.begin(); j != includePragmas.end(); j++) {
		std::string filename = gScope->get_file_info()->get_filenameString();
		size_t delimiter = filename.rfind("/");
		std::string procFilename =
				filename.substr(delimiter + 1);	// erase full qualification (I guess the offset is needed)
		if ((procFilename.compare((*j)->getFilename()) == 0) || ((*j)->getFilename().compare("*") == 0)) {
			this->includes.push_back((*j)->getCode());
			this->insertIncludes = true;
		}
	}
	// select global scope (add the includes to the marker
	select(gScope);
	// no longer add any includes to the markers...
	this->insertIncludes = false;

	// retrieve all nodes marked by the name-matcher
	MasterFunctionalSelector<MarkerAttributeFunctional> mfs = MasterFunctionalSelector<MarkerAttributeFunctional>(
			startNode, MarkerAttributeFunctional(this->nameMatchingSelector->getId()));
	Rose_STL_Container<SgNode*> nodeList = mfs.retrieveNodesWithAttribute();
	Rose_STL_Container<SgNode*>::iterator i;
	std::cout << "Number of nodes found in project AST: " << nodeList.size() << "\nStarting selection processing."
						<< std::endl;
	for (i = nodeList.begin(); i != nodeList.end(); i++) {
		// Start processing of the nodes
		processNode((*i));
	}
}

void GeimerSelector::processNode(SgNode* node) {
	// Create initial state
	t_nodePointers.clear();
	// At each node, the name-matcher marked,
	// look in the specProject for the respective ast fragment
	if (isSgFunctionDefinition(node)) {
		std::vector<std::string> matchesAtNode = nameMatchingSelector->retrieveMatches(node);
		std::vector<std::string>::iterator i;

		// look in the abstract project for the match-corresponding function defs
		Rose_STL_Container<SgNode*> abstractNodes = NodeQuery::querySubTree(this->specProj, V_SgFunctionDefinition);
		Rose_STL_Container<SgNode*>::iterator nIter;

		// we use the saved matches to see which functions apply here
		for (i = matchesAtNode.begin(); i != matchesAtNode.end(); i++) {
			for (nIter = abstractNodes.begin(); nIter != abstractNodes.end(); nIter++) {
				// TODO Think of a better way of achieving the matching than this damn string compare thing
				if (((*i).compare("#") == 0) &&
						(isSgFunctionDefinition(*nIter)->get_declaration()->get_name().getString().compare("$") == 0)) {
					// Check if the filetoken in the datacontainer matches the file_info attached to node
					bool isRoutineToBeInstrumented =
							isRoutineInFileToBeInstrumented("#", isSgLocatedNode(node)->get_file_info()->get_filenameString());

					if (isRoutineToBeInstrumented) {
						//                        std::cout << "routine: #" << " is to be instrumented with:\n" <<
						//                        (*nIter)->unparseToString() << std::endl;
						this->t_nodePointers.push_back(*nIter);
						//                    select(node);
					}
				} else {
					// 2013/01/17 JPL: Yes this has to be an else path
					std::string absStr = isSgFunctionDefinition(*nIter)->get_declaration()->get_name().getString();
					// we have to replace the $ in the absStr by # to get this compare working.
					for (size_t pos = 0; pos < absStr.length(); pos++) {
						if (absStr[pos] == '$') {
							absStr = absStr.replace(pos, 1, "#");
						}
					}

					// Check if the filetoken in the datacontainer matches the file_info attached to node
					if ((*i).compare(absStr) == 0) {
						bool isRoutineToBeInstrumented =
								isRoutineInFileToBeInstrumented((*i), isSgLocatedNode(node)->get_file_info()->get_filenameString());
						if (isRoutineToBeInstrumented) {
							// THIS node corresponds.
							//                            std::cout << "routine: " << (*i) << " is to be instrumented with:\n" <<
							//                            (*nIter)->unparseToString() << std::endl;
							this->t_nodePointers.push_back(*nIter);
							//                      select(node);
						}
					}
				}
			}
		}
		if (t_nodePointers.size() > 0) {
			select(node);
		}
	}
}

SelectionASTMarker* GeimerSelector::createSelectionMarker() {
	// fill with pointers to ast fragments // use t_nodePointer here
	GeimerSelectionASTMarker* marker = new GeimerSelectionASTMarker();
	/*
			std::vector<SgNode*>::iterator i;
			for(i = t_nodePointers.begin(); i != t_nodePointers.end(); i++){
					std::cout << "t_nodePointers: " << (*i)->unparseToString() << std::endl;
			}
	*/
	if (t_nodePointers.size() > 0) {
		marker->setSpecNodes(t_nodePointers);
		//        std::cout << "spec nodes set." << std::endl;
	}
	if (insertIncludes) {
		marker->setIncludeFiles(this->includes);
		//        std::cout << "includes set" << std::endl;
	}
	return marker;
}

// XXX Really needs to be implemented here?
SelectionASTMarker* GeimerSelector::getSelectionMarker(SgNode* node) {}

/** Returns the SgProject* of our abstract-code rose project */
SgProject* GeimerSelector::getSpecProject() { return specProj; }
