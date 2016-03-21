#ifndef INSTRO_UTILITY_LOG_H_
#define INSTRO_UTILITY_LOG_H_

#include <iostream>

namespace InstRO {

enum LogLevel { ERROR, WARN, INFO, DEBUG };

/* Default Log Level */
#ifndef INSTRO_LOG_LEVEL
#define INSTRO_LOG_LEVEL WARN
#endif

/**
 * \brief The visible logLevel is configurable per translation unit.
 * \author Roman Ness
 */
class Logger : public std::ostream {
 public:
	Logger() : std::ostream(std::cout.rdbuf()) {}
	void setLevel(enum LogLevel level) { this->level = level; }

	void setThreshold(enum LogLevel threshold) { this->threshold = threshold; }

 private:
	enum LogLevel level;
	enum LogLevel threshold;

 public:
	template <typename T>
	Logger& operator<<(T const& val) {
		if (level <= threshold) {
			std::cout << val;
		}
		return *this;
	}

	Logger& operator<<(std::ostream&(val)(std::ostream&)) {
		if (level <= threshold) {
			std::cout << val;
		}
		return *this;
	}
};

Logger& logIt(enum LogLevel level, enum LogLevel localThreshold = INSTRO_LOG_LEVEL);

}	// namespace InstRO

#undef INSTRO_LOG_LEVEL

#endif	// INSTRO_UTILITY_LOG_H_
