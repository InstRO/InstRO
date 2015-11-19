#ifndef INSTRO_UTILITY_BWL_FILE_READER
#define INSTRO_UTILITY_BWL_FILE_READER

#include <string>
#include <fstream>
#include <vector>

#include "boost/algorithm/string/predicate.hpp"
#include "boost/algorithm/string.hpp"

namespace InstRO {

namespace Utility {
/*
 * The class handles the reading of a provided black white list secification file.
 */
class BWLFileReader {
 public:
	BWLFileReader(std::string filename);

	/*
	 * Returns the parsed black and whitelist, where the first member of the pair is
	 * the blacklist and the second is the whitelist
	 */
	std::pair<std::vector<std::string>, std::vector<std::string> > getBWList();

	/*
	 * Should this be private?
	 */
 protected:
	void readFile();
	std::string prepareName(std::string name, std::string specifier);

 private:
	std::string wSpecifier, bSpecifier;
	bool hasBeenRead;
	std::string filename;
	std::vector<std::string> blacklist;
	std::vector<std::string> whitelist;
};
}	// end namespace util
}	// end namespace InstRO

#endif
