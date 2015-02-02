#include <instro/selectors/profilebasedselector.h>


using namespace InstRO;

#define VERBOSE 0

/**
  * \brief
  * \param project The project to be traversed
  * \param path The path to a file containing profiling results
  * \param thresholdMillies The threshold in milliseconds from which on methods will be selected
  */
ProfileBasedSelector::ProfileBasedSelector(SgProject* project, std::string path, float thresholdMillies)
	: thresholdMillies(thresholdMillies) {
	// init
	timeScaleParsed = false;
	timeScale = 0;

	parseProfileFromFile(path);
}


void ProfileBasedSelector::visit(SgNode* node) {
	SgFunctionDefinition* funcDef = isSgFunctionDefinition(node);

	if(funcDef) {
		//RN: this is the name without parameters
		std::string funcName = funcDef->get_declaration()->get_name();

		if(hasEnoughRuntime(funcName)) {
			this->select(funcDef);
		}
	}
}


/**
  * \brief Parses the profiling results out of a file
  * \param path The path to a file containing profiling results
  */
void ProfileBasedSelector::parseProfileFromFile(std::string path) {

	std::ifstream file(path.c_str());
	std::string line;

	while(std::getline(file, line)) {

		std::vector<std::string> tokens;

		if(line.empty()) {
			continue;
		}

		// get rid of all those whitespace & parse columns
		boost::trim(line);
		boost::split(tokens, line, boost::is_any_of("\t "), boost::token_compress_on);

		if(tokens.size() >= 7) {
			// the first line with seven tokens should be the header containing time scale
			if(!timeScaleParsed) {
				if(parseTimeScale(tokens[4])) {
					continue;
				} else {
					// don't parse if timescale is invalid anyways
					break;
				}
			}

			// parse name XXX w/o parameters for now
			size_t nameEnd = tokens[6].find_first_of('(');
			std::string name = tokens[6].substr(0,nameEnd);

			Profile prof = {
					boost::lexical_cast<float>(tokens[3]),			// #calls
					boost::lexical_cast<float>(tokens[4])*timeScale,// excl time
					boost::lexical_cast<float>(tokens[5])*timeScale	// incl time
			};
			profileInformation[name] = prof;
		}
	}
}

bool ProfileBasedSelector::parseTimeScale(std::string s) {
	switch(s.at(0)) {

	// ms : millies
	case 'm':
		timeScale=1;
		break;

	// s : seconds
	case 's':
		timeScale=1000;
		break;

	// Ks : 1000 seconds
	case 'K':
		timeScale=1000000;
		break;

	// Ts : the profile has no time scale
	case 'T':
		timeScale=-1;
		std::cout << "Timescale invalid (Ts)." << s << std::endl;
		return false;

	default:
		timeScale=-1;
		std::cout << "Unknown time scale in: " << s << std::endl;
		return false;
	}

	timeScaleParsed=true;
	return true;
}


/**
  *	\brief Returns true iff the given absolute file path is whitelisted
  * \param filePath An absolute file path
  */
bool ProfileBasedSelector::hasEnoughRuntime(std::string funcName) {
	// RN: XXX a more convenient metric would be possible here
	if(profileInformation.find(funcName) == profileInformation.end()) {
		return false;
	}
	return profileInformation.at(funcName).excl > thresholdMillies;
}

void ProfileBasedSelector::dumpProfileInformation() {

	typedef boost::unordered_map<std::string, Profile>::iterator MI;
	for(MI it=profileInformation.begin(); it!=profileInformation.end(); it++) {
		// RN: TODO dump more appropriate
		std::string name = it->first;
		std::cout << name << std::endl;
	}
}
