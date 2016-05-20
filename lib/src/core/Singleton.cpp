#include "instro/Instrumentor.h"
#include "instro/core/Singleton.h"

namespace InstRO {

static InstRO::Instrumentor* instroInstance;
InstRO::Instrumentor* getInstrumentorInstance() { return instroInstance; }
void setInstrumentorInstance(InstRO::Instrumentor* instance) { instroInstance = instance; }
} // InstRO
