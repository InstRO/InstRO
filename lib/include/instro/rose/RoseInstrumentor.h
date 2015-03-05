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
		Rose::RosePassFactory * getFactory(Instrumentor::CompilationPhase phase)
		{
			lockPassManager();
			return new Rose::RosePassFactory(passManager);
		}
//		PassFactory * getFactory(In
		void init(){};
		void apply(){};
		void finalize(){};
	};
	
};
#endif