#ifndef INSTRO_BASE_EXCEPTION_HEADER
#define INSTRO_BASE_EXCEPTION_HEADER

#include <exception>
#include <string>

namespace InstRO{

/**
 * \brief Base class for exceptions thrown by InstRO-Framework
 *
 * \ingroup exceptions
 *
 * \author Jan-Patrick Lehr
 */
class InstroException : public std::exception {

public:

	InstroException(std::string message="Something has gone wrong");
	~InstroException() throw();

	const char* what();

private:
	std::string exceptionMessage;
};

}

#endif
