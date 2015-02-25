#include <instro/contrib/geimerparser.h>

using namespace InstRO;

/*
 * Container for global pragmas
 */
FileParserGlobalPragma::FileParserGlobalPragma(bool includePragma, std::string code){
    this->includePragma = includePragma;
    this->code = code;
}

FileParserGlobalPragma::FileParserGlobalPragma(bool includePragma, std::string code, std::string fileToken){
    this->includePragma = includePragma;
    this->code = code;
    this->file = fileToken;
}

std::string FileParserGlobalPragma::getCode(){
    return this->code;
}

bool FileParserGlobalPragma::isIncludePragma(){
    return this->includePragma;
}

void FileParserGlobalPragma::setCode(std::string code){
    this->code = code;
}

std::string FileParserGlobalPragma::getFilename(){
    return this->file;
}

/*
 * Container for scoped pragmas (a wildcard pragma is a scoped pragma as well)
 */
FileParserScopedPragma::FileParserScopedPragma(std::string routine, std::string code, d_Type type){
    this->routine = routine;
    this->code = code;
    this->type = type;
}

FileParserScopedPragma::FileParserScopedPragma(std::string routine, std::string code, d_Type type, std::string fileToken){
    this->routine = routine;
    this->code = code;
    this->type = type;
    this->file = fileToken;
}

std::string FileParserScopedPragma::getRoutine(){
    return this->routine;
}

std::string FileParserScopedPragma::getCode(){
    return this->code;
}

void FileParserScopedPragma::setCode(std::string code){
    this->code = code;
}

d_Type FileParserScopedPragma::getType(){
    return this->type;
}

std::string FileParserScopedPragma::getFilename(){
    return this->file;
}

bool FileParserScopedPragma::operator<(const FileParserScopedPragma& arg) const {
    return (type < arg.type);
}


/** GeimerConfigurationController */
/**
 * This is the constructor. A filename is required as the filename is straightly passed to the parser
 * and the abstract code generator. Therefore if no filename is given this thing exits with -1
 */
GeimerConfigurationController::GeimerConfigurationController(const char* fileName){
    // Constructor
    this->pFilename = std::string(fileName);
    if(this->pFilename.empty()) exit(-1);
    parser = GeimerConfigurationParser(fileName);
    generator = GeimerAbstractCodeGenerator(fileName, &parser);
}

/**
 * The run method simply runs parsing and generating abstract code.
 */
void GeimerConfigurationController::run(){
    parser.parseFile();
    generator.generate();
    this->generatedFilename = generator.getGeneratedFilename();
}

/**
 * Returns the filename the abstract code generator generated.
 */
std::string GeimerConfigurationController::getGeneratedFilename(){
    return this->generatedFilename;
}

/**
 * Method to check whether we have a rule for \param routinename to be instrumented in \param filename
 */
bool GeimerConfigurationController::isRoutineInFileToBeInstrumented(std::string routinename, std::string filename){
    //std::cout << __FUNCTION__ << ": params: " << routinename << " , " << filename << std::endl;
    if(routinename.compare("#") == 0){
        // this is the global case.
        // we have to use the wildcard pragmas. XXX Maybe change that in the future (only one datastructure...)
        std::vector<FileParserScopedPragma*> wcp = parser.getWildcardPragmas();
        std::vector<FileParserScopedPragma*>::iterator wcpElem;
        for(wcpElem = wcp.begin(); wcpElem != wcp.end(); wcpElem++){
            if((*wcpElem)->getFilename().compare("*") == 0 || (*wcpElem)->getFilename().compare(filename) == 0){
//                std::cout << routinename << " in " << filename << " said to be instrumented" << std::endl;
                return true;
            }
        }

    } else {
        std::map<std::string, std::vector<FileParserScopedPragma> > rMap = parser.getScopedPragmaMap();
        std::map<std::string, std::vector<FileParserScopedPragma> >::iterator elem;
        elem = rMap.find(routinename);
        if(elem == rMap.end()){
            return NULL; // Case of not found (Which should NOT happen)
        }

        std::vector<FileParserScopedPragma>::iterator vecElem = (*elem).second.begin();
        for(; vecElem != elem->second.end(); vecElem++){
//            std::cout << "Vector element: " << (*vecElem).getFilename() << std::endl;
            if((*vecElem).getFilename().compare("*") == 0 || (*vecElem).getFilename().compare(filename) == 0){
//                std::cout << routinename << " in " << filename << " said to be instrumented" << std::endl;
                return true; // if correspondence found return true
            }
        }
    }
    return false; // else false
}

/**
 * Returns a std::list of all routinenames which should be regarded somehow.
 */
std::list<std::string> GeimerConfigurationController::getWhiteList(){
    std::list<std::string> list;
    std::vector<std::string> v = this->parser.getAllRoutineNames();
    list.assign(v.begin(), v.end());
    return list;
}

/**
 * Returns a vector of all include statements we have to regard.
 */
std::vector<FileParserGlobalPragma*> GeimerConfigurationController::getIncludePragmas(){
    return this->parser.getIncludePragmas();
}

/*
 * GeimerConfigurationParser
 */
GeimerConfigurationParser::GeimerConfigurationParser(const char* file){
    this->m_pFileName = file;
    actualLine = 0;
    actualCol = 0;
    functionToSpecificationMap = std::map<std::string, std::vector<FileParserScopedPragma> >(); // Holds scoped pragmas per function name
    globalPragmas = std::vector<FileParserGlobalPragma*>();
    wildcardPragmas = std::vector<FileParserScopedPragma*>();
    includePragmas = std::vector<FileParserGlobalPragma*>();
}

/**
 * Reads the next line from the input file. Skips comment lines.
 */
std::string GeimerConfigurationParser::getNextLine(std::ifstream& inFile){
    std::string returnLine;
    while(inFile.good()){
        getline(inFile, returnLine);
        actualLine++;
        if(returnLine.empty()) continue;
        // erase comment lines
        if(returnLine.at(0) == '/' && returnLine.at(1) == '/'){
            continue;
        } else {
            break;
        }
    }

    int checkQuote = 0;
    int curCol = 0;
    // 2013/01/13 JPL: Added a very basic warning about (potentially) missing quotation marks
    while(curCol < returnLine.length()){
        if(returnLine.at(curCol) == '\"')
            checkQuote++;
        curCol++;
    }
    if(checkQuote % 2 != 0){
        std::cerr << "Warning: in line " << actualLine << " could be missing a quotation mark." << std::endl;
    }

    actualCol = 0;
    line = returnLine;
    return returnLine;
}

/**
 * Fetches the next token from the read line.
 */
std::string GeimerConfigurationParser::getNextToken(){
    std::string token;

    int start = actualCol;
    actualCol += 1;
    bool hasQuotes = false;

    while(actualCol < line.length()){
        if(line.at(actualCol) == '\"'){
            if(line.at(actualCol-1) == '\\'){
                actualCol++;
                continue;
            }
            hasQuotes = !hasQuotes;
        }

        if(!hasQuotes){
            if(line.at(actualCol) == ' '){ break;}
            if(line.at(actualCol) == '='){ break;}
        }
        actualCol++;
    }
    if(start != 0) start++;
    token = line.substr(start, actualCol-start);
    if(token.at(0) == '\"')
        token = token.substr(1);
    if(token.at(token.size()-1) == '"')
        token = token.substr(0, token.size()-1);
    return token;
}

/**
 * Parses the input file.
 */
void GeimerConfigurationParser::parseFile(){
    std::cout << "Inputfile for GeimerParser: " << m_pFileName << std::endl;
    // open file
    std::ifstream inFile;
    inFile.open(this->m_pFileName);

    // get line by line
    while(inFile.good()){
	std::string lineToProcess = getNextLine(inFile);

	if(lineToProcess.empty()){
            // leave the loop as there is nothing more to be parsed
            continue;
	}

	// parse line and construct containers
	std::string token = getNextToken();
	if(token.compare("file") == 0){
            std::string fileToken = getNextToken();
            // process global things
            token = getNextToken();
            if(token.compare("include") == 0){
                std::string code = getNextToken();
                includePragmas.push_back(new FileParserGlobalPragma(true, code, fileToken));
            } else if(token.compare("global_initializer") == 0){
                std::string code = getNextToken();
                globalPragmas.push_back(new FileParserGlobalPragma(false, code, fileToken));
            } else if(token.compare("main_initializer") == 0){
                // JPL: Do we need the file name here?
                // JPL: Yes, we do need it!
                std::string code = getNextToken();
                if(functionToSpecificationMap.count("#main") > 0){
                    functionToSpecificationMap.find("#main")->second.push_back(FileParserScopedPragma("#main", code, MAININIT, fileToken));
                } else {
                    std::vector<FileParserScopedPragma> localVec = std::vector<FileParserScopedPragma>();
                    localVec.push_back(FileParserScopedPragma("#main", code, MAININIT, fileToken));
                    functionToSpecificationMap.insert(std::pair<std::string, std::vector<FileParserScopedPragma> >("#main", localVec));
                }
            } else {
                std::cerr << "Something went wrong while processing line: " << actualLine << std::endl;
            }
        } else {
            d_Type readType = UNDEF; // to memorize the read token type

            if(token.compare("decl") == 0){
                readType = DECL;
            } else if(token.compare("entry") == 0){
                readType = ENTRY;
            } else if(token.compare("abort") == 0){
                readType = ABORT;
            } else if(token.compare("exit") == 0){
                readType = EXIT;
            }
            if(readType != UNDEF){
                token = getNextToken();
                if(token.compare("file") == 0){
                    std::string fileToken = getNextToken();
                    if(getNextToken().compare("routine") == 0){
                        std::string routineToken = getNextToken();
                        if(getNextToken().compare("code") == 0){
                            std::string code = getNextToken();
                            if(routineToken.compare("#") == 0){
                                wildcardPragmas.push_back(new FileParserScopedPragma(routineToken, code, readType, fileToken));
                            } else {
                                // Look up Function name in Map and add to specific vector
                                if(functionToSpecificationMap.count(routineToken) > 0){
                                    functionToSpecificationMap.find(routineToken)->second.push_back(FileParserScopedPragma(routineToken, code, readType, fileToken));
                                } else {
                                    std::vector<FileParserScopedPragma> localVec = std::vector<FileParserScopedPragma>();
                                    localVec.push_back(FileParserScopedPragma(routineToken, code, readType, fileToken));
                                    functionToSpecificationMap.insert(std::pair<std::string, std::vector<FileParserScopedPragma> >(routineToken, localVec));
                                }
                                // scopedPragmas.push_back(new FileParserScopedPragma(routineToken, code, DECL));
                            }
                        }
                    }
                }

            }
            /*
        2013/01/13: JPL: Cleaned this ugly code up to the several lines above
                    Will be completely removed in later iterations.

        else if(token.compare("entry") == 0){
            token = getNextToken();
            if(token.compare("file") == 0){
                std::string fileToken = getNextToken();
                if(getNextToken().compare("routine") == 0){
                    std::string routineToken = getNextToken();
                    if(getNextToken().compare("code") == 0){
                        std::string code = getNextToken();
                        if(routineToken.compare("#") == 0){
                            wildcardPragmas.push_back(new FileParserScopedPragma(routineToken, code, ENTRY, fileToken));
                        } else {
                            // Look up function name in map and add to specific vector
                            if(functionToSpecificationMap.count(routineToken) > 0){
                                functionToSpecificationMap.find(routineToken)->second.push_back(FileParserScopedPragma(routineToken, code, ENTRY, fileToken));
                            } else {
                                std::vector<FileParserScopedPragma> localVec = std::vector<FileParserScopedPragma>();
                                localVec.push_back(FileParserScopedPragma(routineToken, code, ENTRY, fileToken));
                                // const std::pair<std::string, std::vector<FileParserScopedPragma*> > constPair = std::pair<std::string, std::vector<FileParserScopedPragma*>(routineToken, localVec);
                                functionToSpecificationMap.insert(std::pair<std::string, std::vector<FileParserScopedPragma> >(routineToken, localVec));
                            }
                            // scopedPragmas.push_back(new FileParserScopedPragma(routineToken, code, ENTRY));
                        }
                    }
                }
            }
 } else if(token.compare("abort") == 0){
            token = getNextToken();
            if(token.compare("file") == 0){
                std::string fileToken = getNextToken();
                if(getNextToken().compare("routine") == 0){
                    std::string routineToken = getNextToken();
                    if(getNextToken().compare("code") == 0){
                        std::string code = getNextToken();
                        if(routineToken.compare("#") == 0){
                            wildcardPragmas.push_back(new FileParserScopedPragma(routineToken, code, ABORT, fileToken));
                        } else {
                            // Look up function name in map and add to specific vector
                            if(functionToSpecificationMap.count(routineToken) > 0){
                                functionToSpecificationMap.find(routineToken)->second.push_back(FileParserScopedPragma(routineToken, code, ABORT, fileToken));
                            } else {
                                std::vector<FileParserScopedPragma> localVec = std::vector<FileParserScopedPragma>();
                                localVec.push_back(FileParserScopedPragma(routineToken, code, ABORT, fileToken));
                                // const std::pair<std::string, std::vector<FileParserScopedPragma*> > constPair = std::pair<std::string, std::vector<FileParserScopedPragma*>(routineToken, localVec);
                                functionToSpecificationMap.insert(std::pair<std::string, std::vector<FileParserScopedPragma> >(routineToken, localVec));
                            }
                            // scopedPragmas.push_back(new FileParserScopedPragma(routineToken, code, ABORT));
                        }
                    }
                }
            }
 } else if(token.compare("exit") == 0){
            token = getNextToken();
            if(token.compare("file") == 0){
                std::string fileToken = getNextToken();
                if(getNextToken().compare("routine") == 0){
                    std::string routineToken = getNextToken();
                    if(getNextToken().compare("code") == 0){
                        std::string code = getNextToken();
                        if(routineToken.compare("#") == 0){
                            wildcardPragmas.push_back(new FileParserScopedPragma(routineToken, code, EXIT, fileToken));
                        } else {
                            // Look up function name in map and add to specific vector
                            if(functionToSpecificationMap.count(routineToken) > 0){
                                functionToSpecificationMap.find(routineToken)->second.push_back(FileParserScopedPragma(routineToken, code, EXIT, fileToken));
                            } else {
                                std::vector<FileParserScopedPragma> localVec = std::vector<FileParserScopedPragma>();
                                localVec.push_back(FileParserScopedPragma(routineToken, code, EXIT, fileToken));
                                // const std::pair<std::string, std::vector<FileParserScopedPragma*> > constPair = std::pair<std::string, std::vector<FileParserScopedPragma*>(routineToken, localVec);
                                functionToSpecificationMap.insert(std::pair<std::string, std::vector<FileParserScopedPragma> >(routineToken, localVec));
                            }
                            // scopedPragmas.push_back(new FileParserScopedPragma(routineToken, code, EXIT));
                        }
                    }
                }
            }
        }
*/
            else {
                std::cerr << "Something went wrong in line: " << actualLine << std::endl;
            }
            // while loop end
        }
    }
    if(functionToSpecificationMap.count("main") == 0){
        std::vector<FileParserScopedPragma> v;
        v.push_back(FileParserScopedPragma("main", "", UNDEF, ""));
        functionToSpecificationMap.insert(std::pair<std::string, std::vector<FileParserScopedPragma> >("main", v));
    }

    inFile.close();

    buildSubstitutions(); // TODO find a better way?

    // Sort - necessary for simple backend
    std::map<std::string, std::vector<FileParserScopedPragma> >::iterator i;
    // sort each vector.
    for(i = this->functionToSpecificationMap.begin(); i != this->functionToSpecificationMap.end(); i++){
        std::sort(i->second.begin(), i->second.end());
    }
}


/*
 * Little helper to substitute std::string "pattern" with "substitute" in std::string "target"
 *
 * TODO JPL: Check if this method should go to some "utility" class/namespace
 *
 */
std::string GeimerConfigurationParser::substituteString(std::string target, std::string pattern,  const std::string substitute){
    int start = target.find(pattern);
    while(start != std::string::npos){
        target = target.replace(start, pattern.length(), substitute);//, substitute.length()-1);
        start = target.find(pattern);
    }
    return target;
}

/*
 * Substitute the used keywords with calls to some pseudo functions.
 * This is absolutely ugly.. better ideas?
 */
void GeimerConfigurationParser::buildSubstitutions(){
    // build substitutions for global and scoped pragmas

    std::vector<FileParserScopedPragma*>::iterator scopeIter;
    for(scopeIter = wildcardPragmas.begin(); scopeIter != wildcardPragmas.end(); scopeIter++){
        (*scopeIter)->setCode(substituteString((*scopeIter)->getCode(), "@ROUTINE@", "__INSTR_FUNC_NAME__"));
        (*scopeIter)->setCode(substituteString((*scopeIter)->getCode(), "@FILE@", "__FILE__"));
	
        (*scopeIter)->setCode(substituteString((*scopeIter)->getCode(), "@LINE@", "__LINE__"));

        (*scopeIter)->setCode(substituteString((*scopeIter)->getCode(), "@COL@", "_instrumentor_get_col()"));

        (*scopeIter)->setCode(substituteString((*scopeIter)->getCode(), "@BEGIN_LINE@", "_instrumentor_get_begin_line()"));

        (*scopeIter)->setCode(substituteString((*scopeIter)->getCode(), "@BEGIN_COL@", "_instrumentor_get_begin_col()"));
        (*scopeIter)->setCode(substituteString((*scopeIter)->getCode(), "@END_LINE@", "_instrumentor_get_end_line()"));
        (*scopeIter)->setCode(substituteString((*scopeIter)->getCode(), "@END_COL@", "_instrumentor_get_end_col()"));
        (*scopeIter)->setCode(substituteString((*scopeIter)->getCode(), "@RTTI@", "_instrumentor_get_rtti()"));
        (*scopeIter)->setCode(substituteString((*scopeIter)->getCode(), "@ARGC@", "argc"));
        (*scopeIter)->setCode(substituteString((*scopeIter)->getCode(), "@ARGV@", "argv"));
        (*scopeIter)->setCode(substituteString((*scopeIter)->getCode(), "\\\"", "\""));
    }

    /* Because the new map is introduced */
    std::map<std::string, std::vector<FileParserScopedPragma> >::iterator mapIterator;
    std::vector<FileParserScopedPragma>::iterator scopeIter2;
    for(mapIterator = functionToSpecificationMap.begin(); mapIterator != functionToSpecificationMap.end(); mapIterator++){
        std::vector<FileParserScopedPragma> scopePragmas = mapIterator->second;
        //	for(scopeIter = scopedPragmas.begin(); scopeIter != scopedPragmas.end(); scopeIter++){
        for(scopeIter2 = scopePragmas.begin(); scopeIter2 != scopePragmas.end(); scopeIter2++){

            (*scopeIter2).setCode(substituteString((*scopeIter2).getCode(), "@ROUTINE@", "__INSTR_FUNC_NAME__"));
            (*scopeIter2).setCode(substituteString((*scopeIter2).getCode(), "@FILE@", "__FILE__"));

            (*scopeIter2).setCode(substituteString((*scopeIter2).getCode(), "@LINE@", "__LINE__"));

            (*scopeIter2).setCode(substituteString((*scopeIter2).getCode(), "@COL@", "_instrumentor_get_col()"));

            (*scopeIter2).setCode(substituteString((*scopeIter2).getCode(), "@BEGIN_LINE@", "_instrumentor_get_begin_line()"));

            (*scopeIter2).setCode(substituteString((*scopeIter2).getCode(), "@BEGIN_COL@", "_instrumentor_get_begin_col()"));
            (*scopeIter2).setCode(substituteString((*scopeIter2).getCode(), "@END_LINE@", "_instrumentor_get_end_line()"));
            (*scopeIter2).setCode(substituteString((*scopeIter2).getCode(), "@END_COL@", "_instrumentor_get_end_col()"));
            (*scopeIter2).setCode(substituteString((*scopeIter2).getCode(), "@RTTI@", "_instrumentor_get_rtti()"));
            (*scopeIter2).setCode(substituteString((*scopeIter2).getCode(), "@ARGC@", "argc"));
            (*scopeIter2).setCode(substituteString((*scopeIter2).getCode(), "@ARGV@", "argv"));
            (*scopeIter2).setCode(substituteString((*scopeIter2).getCode(), "\\\"", "\""));
        }
        mapIterator->second = scopePragmas;
    }
}

/** Returns a vector of the include things */
std::vector<FileParserGlobalPragma*>& GeimerConfigurationParser::getIncludePragmas(){
    return this->includePragmas;
}

/** Returns a vector of all global things */
std::vector<FileParserGlobalPragma*>& GeimerConfigurationParser::getGlobalPragmas(){
    return this->globalPragmas;
}

/** Returns a vector of all (method-name) wildcard things */
std::vector<FileParserScopedPragma*>& GeimerConfigurationParser::getWildcardPragmas(){
    return this->wildcardPragmas;
}

/** Returns a std::map holding routinenames as key and a vector of all scoped pragmas for this routine */
std::map<std::string, std::vector<FileParserScopedPragma> > GeimerConfigurationParser::getScopedPragmaMap(){
    return this->functionToSpecificationMap;
}

/** Returns a std::vector of all routine names we have some rule for*/
std::vector<std::string> GeimerConfigurationParser::getAllRoutineNames(){
    std::vector<std::string> retVec;
    std::map<std::string, std::vector<FileParserScopedPragma> >::iterator i;
    for(i = this->functionToSpecificationMap.begin(); i != this->functionToSpecificationMap.end(); i++){
        retVec.push_back((*i).first);
    }
    if(wildcardPragmas.size()>0){
        retVec.push_back("#");
    }
    return retVec;
}

/**
 * The GeimerAbstractCodeGenerator generates the specification file, which is then passed to our instrumentor.
 * It uses the data containers provided by the UserConfigurationFileParser.
 * filename should be the filename of the configuration file passed to the UserConfigurationFileParser ucfp
 * object.
 *
 */

GeimerAbstractCodeGenerator::GeimerAbstractCodeGenerator(const char* filename, GeimerConfigurationParser* gConfParser){
    this->m_pFilename = filename;
    this->m_ucfp = gConfParser;
    if(this->m_ucfp->getIncludePragmas().size() >0){
        std::cout << "size was greater zero" << std::endl;
    }
    std::string t_outfile = std::string(m_pFilename);
    size_t delimiter = t_outfile.rfind("/");
    m_outfileName = t_outfile;
    m_outfileName.insert(delimiter+1, "_gen_specification_");
    m_outfileName.append(".cpp");
}

/*
 * Little helper to substitute std::string "pattern" with "substitute" in std::string "target"
 *
 * TODO JPL: Check if this method should go to some "utility" class/namespace
 *
 */
std::string GeimerAbstractCodeGenerator::substituteString(std::string target, std::string pattern,  const std::string substitute){
    std::string returnString;
    int start = target.find(pattern);
    while(start != std::string::npos){
        target.replace(start, pattern.length(), substitute);//, substitute.length()-1);
        start = target.find(pattern);
    }

    return target;
}

/**
 * Generates the code and writes it to file.
 * This is a very straightforward approach.
 */
void GeimerAbstractCodeGenerator::generate(){
//    std::cout << m_outfileName << std::endl;
    int lN = 0; // holds the actual line number.
    // create file
    std::fstream fileStream;

    // only output and discarding everythin actually in the file
    fileStream.open(m_outfileName.c_str(), std::fstream::out | std::fstream::trunc);

    // Some information to the user...
    std::string dev_string;
    dev_string.append("/* This file is tool generated for correctnes. You can use it for debugging purposes.*/\n\n");
    fileStream << dev_string;

    // generate include statements
    std::string gs_string;
    std::vector<FileParserGlobalPragma*> includes = m_ucfp->getIncludePragmas();
    std::vector<FileParserGlobalPragma*>::iterator includeIter;
    std::string hackString;

    for(includeIter = includes.begin(); includeIter != includes.end(); includeIter++){
        if(includeIter == includes.begin()){
            gs_string.append("#pragma INSTRUMENTOR INCLUDES=\n");
            lN++;
        }
        hackString.append((*includeIter)->getCode());
        hackString.append(" ");
        gs_string.append("#include \"");
        gs_string.append((*includeIter)->getCode());
        gs_string.append("\"\n");
        lN++;
    }
    if(includes.size()>0){
        gs_string.insert(30, hackString);
    }
    gs_string.append("\n");
    lN++;

    // generate global declarations
    std::vector<FileParserGlobalPragma*> globals = m_ucfp->getGlobalPragmas();
    std::vector<FileParserGlobalPragma*>::iterator globalIter;
    for(globalIter = globals.begin(); globalIter != globals.end(); globalIter++){
        if(globalIter == globals.begin()){
            gs_string.append("#pragma INSTRUMENTOR GLOBAL_INITIALIZER\n");
            lN++;
        }
        gs_string.append((*globalIter)->getCode());
        gs_string.append("\n");
        lN++;
    }
    // write globals to file
    fileStream << gs_string;


    // generate wildcard statements
    std::string ws_string;
    std::vector<FileParserScopedPragma*> wildcards = m_ucfp->getWildcardPragmas();
    std::vector<FileParserScopedPragma*>::iterator wildIter;
    d_Type lType = UNDEF;
    if(wildcards.size() > 0){
	for(wildIter = wildcards.begin(); wildIter != wildcards.end(); wildIter++){
            if(wildIter == wildcards.begin()){
                ws_string.append("#pragma INSTRUMENTOR ROUTINE = $\n");
                lN++;
                ws_string.append("void $(){\n");
                lN++;
                ws_string.append("char __INSTR_FUNC_NAME__[]=\"$$\";\n");
                lN++;
            }
            if((*wildIter)->getType() != lType && (*wildIter)->getType() == DECL){
                ws_string.append("#pragma INSTRUMENTOR VARDECL\n");
                lN++;
                lType = DECL;
            }
            if((*wildIter)->getType() != lType && (*wildIter)->getType() == ENTRY){
                ws_string.append("#pragma INSTRUMENTOR ENTRY\n");
                lN++;
                lType = ENTRY;
            }
            if((*wildIter)->getType() != lType && (*wildIter)->getType() == ABORT){
                ws_string.append("#pragma INSTRUMENTOR ABORT\n");
                lN++;
                lType = ABORT;
            }
            if((*wildIter)->getType() != lType && (*wildIter)->getType() == EXIT){
                ws_string.append("#pragma INSTRUMENTOR EXIT\n");
                lN++;
                lType = EXIT;
            }
            ws_string.append("\t");
            ws_string.append((*wildIter)->getCode());
            ws_string.append("\n");
            lN++;
	}
	ws_string.append("}\n");
	lN++;
	
	fileStream << ws_string;
    }
    // generate scoped statements
    std::string ss_string;
    std::string lRoutine;

    std::map<std::string, std::vector<FileParserScopedPragma> > scopesMap = m_ucfp->getScopedPragmaMap();
    std::map<std::string, std::vector<FileParserScopedPragma> >::iterator scopeMapIter;
    for(scopeMapIter = scopesMap.begin(); scopeMapIter != scopesMap.end(); scopeMapIter++){

        std::vector<FileParserScopedPragma> scopes = scopeMapIter->second; //m_ucfp->getScopedPragmas();
        std::vector<FileParserScopedPragma>::iterator scopeIter;
	for(scopeIter = scopes.begin(); scopeIter != scopes.end(); scopeIter++){
            std::string routine = (*scopeIter).getRoutine(); // holds the routine name.

            // XXX If this is moved to some utility thing, adapt!!
            routine = substituteString(routine, "#", "$"); // build wildcards

            if(lRoutine.compare(routine) != 0){
                std::string routineString = "";
                if(routine.compare("main") == 0){
                    routineString.append("int ");
                    routineString.append(routine);
                    routineString.append("(int argc, char** argv)");
                } else {
                    routineString.append("void ");
                    routineString.append(routine);
                    routineString.append("()");
                }
                ss_string.append("#pragma INSTRUMENTOR ROUTINE =");
                lN++;
                ss_string.append(routineString);
                ss_string.append("\n");
                lN++;
                ss_string.append(routineString);
                ss_string.append("{\n");
                lN++;
                ss_string.append("char __INSTR_FUNC_NAME__[]=\"");
                ss_string.append(routine);
                ss_string.append("\";\n");
                lN++;
                lRoutine = routine;
            }
            if((*scopeIter).getType() == MAININIT){
                ss_string.append("#pragma INSTRUMENTOR MAIN_INITIALIZER\n");
                lN++;
            }
            if((*scopeIter).getType() == DECL){
                ss_string.append("#pragma INSTRUMENTOR VARDECL\n");
                lN++;
            }
            if((*scopeIter).getType() == ENTRY){
                ss_string.append("#pragma INSTRUMENTOR ENTRY\n");
                lN++;
            }
            if((*scopeIter).getType() == EXIT){
                ss_string.append("#pragma INSTRUMENTOR EXIT\n");
                lN++;
            }
            if((*scopeIter).getType() == ABORT){
                ss_string.append("#pragma INSTRUMENTOR ABORT\n");
                lN++;
            }
            ss_string.append("\t");
            ss_string.append((*scopeIter).getCode());
            ss_string.append("\n");
            lN++;
	}
        if(scopeMapIter->first.compare("main") == 0){
            if(m_ucfp->getWildcardPragmas().size()>0){
                ss_string.append("$();\n");
            }
            // Add a call to every function
            std::map<std::string, std::vector<FileParserScopedPragma> >::iterator locFunctionIterator;
            for(locFunctionIterator = scopesMap.begin(); locFunctionIterator != scopesMap.end(); locFunctionIterator++){
                std::string fun = (*locFunctionIterator).first;
                fun = substituteString(fun, "#", "$");
                if(fun.compare("main") == 0){
                    continue;
                } else {
                    ss_string.append(fun);
                    ss_string.append("();\n");
                }
            }
            ss_string.append("return 0;\n");
	}
	ss_string.append("}\n");
    }
    lN++;

#ifdef DEBUGSELECTOR
    std::cout << lN << " lines written to " << m_outfileName << std::endl;
#endif

    fileStream << ss_string;

    fileStream.close();
}

/** Returns the filename the Abstract Code Generator wrote to. */
std::string GeimerAbstractCodeGenerator::getGeneratedFilename(){
    return m_outfileName;
}
