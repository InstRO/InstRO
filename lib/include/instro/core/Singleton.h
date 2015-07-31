#ifndef INSTRO_SINGLETON_H
#define INSTRO_SINGLETON_H

#include "instro/core/Instrumentor.h"

namespace InstRO {

InstRO::Instrumentor* getInstrumentorInstance();
void setInstrumentorInstance(InstRO::Instrumentor* instance);
}

#endif // INSTRO_SINGLETON_H
