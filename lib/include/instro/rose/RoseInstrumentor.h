#ifndef INSTRO_ROSE_INSTRUMENTOR_H
#define INSTRO_ROSE_INSTRUMENTOR_H

#include "instro/core/Instrumentor.h"
// #include "instro/core/PassFactory.h"

#include "instro/rose/RosePassFactory.h"

namespace InstRO
{
	namespace Rose{
	class RosePassFactory;
	}
	class RoseInstrumentor: public Instrumentor
	{
	public: 
		PassFactory * getFactory(Instrumentor::CompilationPhase phase){return dynamic_cast<PassFactory*>(new Rose::RosePassFactory(passManager));}
//		PassFactory * getFactory(In
		void init(){};
		void apply(){};
		void finalize(){};
	};
	
};
#endif