#ifndef INSTRO_GEIMERPARSER_H___
#define INSTRO_GEIMERPARSER_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <list>

namespace InstRO{


enum d_Type {UNDEF, DECL, ENTRY, EXIT, ABORT, MAININIT};

// Datastructures /

/**
 * Datastructure to store certain information which is needed in code generation
 * for (global) scope pragmas.
 * \ingroup Util
 * \author Jan-Patrick Lehr
 */
class FileParserGlobalPragma // : public FileParserPragma {
{
public:
    FileParserGlobalPragma(bool includePragma, std::string code);
    FileParserGlobalPragma(bool includePragma, std::string code, std::string file="*");
    bool isIncludePragma();
    std::string getCode();
    void setCode(std::string code);
    std::string getFilename();

private:
    bool includePragma;
    std::string code;
    std::string file; // JPL: We need to store the filename where to insert!
};

/**
 * Datastructure to store certain information which is needed in code generation
 * for (local) scoped pragmas.
 * \ingroup Util
 * \author Jan-Patrick Lehr
 */
class FileParserScopedPragma // : public FileParserPragma {
{
public:
    FileParserScopedPragma(std::string routine = "main", std::string code = "", d_Type type = DECL);
    FileParserScopedPragma(std::string routine = "main", std::string code = "", d_Type type = DECL, std::string file="*");
    std::string getRoutine();
    std::string getCode();
    void setCode(std::string code);
    d_Type getType();
    std::string getFilename();
    bool operator<(const FileParserScopedPragma& arg) const;

private:
    std::string file; // JPL: We have to use this data as well!
    std::string routine;
    std::string code;
    d_Type type;
};

// end Datastructures

// Parser


/**
 * Parses the given configuration file and creates datacontainers from which code can
 * be generated.
 * \ingroup Util
 * \author Jan-Patrick Lehr
 */
class GeimerConfigurationParser {

public:
    GeimerConfigurationParser(){};
    ~GeimerConfigurationParser(){};
    GeimerConfigurationParser(const char* file);
    void parseFile();
    std::vector<FileParserGlobalPragma*>& getIncludePragmas();
    std::vector<FileParserGlobalPragma*>& getGlobalPragmas();
    std::vector<FileParserScopedPragma*>& getWildcardPragmas();
    std::map<std::string, std::vector<FileParserScopedPragma> > getScopedPragmaMap();
    std::vector<std::string> getAllRoutineNames();
private:
    const char* m_pFileName;
    std::string line;
    int actualLine;
    int actualCol;
    std::map<std::string, std::vector<FileParserScopedPragma> > functionToSpecificationMap;

    std::vector<FileParserGlobalPragma*> globalPragmas;
    std::vector<FileParserGlobalPragma*> includePragmas;
    std::vector<FileParserScopedPragma*> wildcardPragmas;

    std::string getNextLine(std::ifstream& inFile);
    std::string getNextToken();
    void buildSubstitutions();
    std::string substituteString(std::string target, std::string pattern, const std::string substitute);
};

// Abstract Code Generator /

/**
 * This class generates code from the parsed things 
 * \ingroup Util
 * \author Jan-Patrick Lehr
 */
class GeimerAbstractCodeGenerator {
public:
    GeimerAbstractCodeGenerator(){};
    GeimerAbstractCodeGenerator(const char* filename, GeimerConfigurationParser* ucfp);
    void generate();
    std::string getGeneratedFilename();
private:
    const char* m_pFilename;
    std::string m_outfileName;
    GeimerConfigurationParser* m_ucfp;
    std::string substituteString(std::string target, std::string pattern,  const std::string substitute);
};


/**
 * Entity to control the parsing and abstract code generation.
 * Is used by the Selector to retrieve necessary informations during selection phase.
 * \ingroup Util
 * \author Jan-Patrick Lehr
 */
class GeimerConfigurationController {
public:
    GeimerConfigurationController(const char* fileName);
    void run();
    std::string getGeneratedFilename();
    std::list<std::string> getWhiteList();
    std::vector<FileParserGlobalPragma*> getIncludePragmas();
    bool isRoutineInFileToBeInstrumented(std::string routinename, std::string filename);
private:
    std::string pFilename;
    std::string generatedFilename;
    GeimerConfigurationParser parser;
    GeimerAbstractCodeGenerator generator;
};
}
#endif
