
#include "instro/utility/Logger.h"

namespace InstRO {

static Logger logger;

Logger& logIt(enum LogLevel level, enum LogLevel localThreshold) {
	logger.setLevel(level);
	logger.setThreshold(localThreshold);

	if (level == ERROR) {
		std::cout << "[Error] ";
	}

	return logger;
}

}	// namespace InstRO
