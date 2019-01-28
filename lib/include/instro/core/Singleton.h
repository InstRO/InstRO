#ifndef INSTRO_CORE_SINGLETON_H
#define INSTRO_CORE_SINGLETON_H

namespace InstRO {
class Instrumentor;

InstRO::Instrumentor* getInstrumentorInstance();
void setInstrumentorInstance(InstRO::Instrumentor* instance);
}	// namespace InstRO

#endif	// INSTRO_SINGLETON_H
