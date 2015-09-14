#include "instro/exception/instroexception.h"

using namespace InstRO;

InstroException::InstroException(std::string message) : exceptionMessage(message) {}

InstroException::~InstroException() throw() {}

const char* InstroException::what() { return exceptionMessage.c_str(); }
