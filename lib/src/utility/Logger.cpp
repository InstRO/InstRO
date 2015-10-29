
#include "instro/utility/Logger.h"

namespace InstRO {

static Logger logger;

Logger& logIt(enum LogLevel level, enum LogLevel localThreshold) {
    logger.setLevel(level);
    logger.setThreshold(localThreshold);
    return logger;
}

}	// namespace InstRO
