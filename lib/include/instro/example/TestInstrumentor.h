#ifndef INSTRO_ROSE_INSTRUMENTOR_H
#define INSTRO_ROSE_INSTRUMENTOR_H

#include "instro/core/Instrumentor.h"
// #include "instro/core/PassFactory.h"

#include "instro/example/ExamplePassFactory.h"

namespace InstRO
{
	namespace Example{
	class TestPassFactory;
	}
	class TestInstrumentor: public Instrumentor
	{
	public: 
		Example::TestPassFactory * getFactory(Instrumentor::CompilationPhase phase){ return new Example::TestPassFactory(passManager); }
		// TestPassFactory * getFactory(Instrumentor::CompilationPhase phase){return dynamic_cast<PassFactory*>(new Test::TestPassFactory(passManager));}
//		PassFactory * getFactory(In
		void init(){};
		void apply(){};
		void finalize(){};
	};
	
};
#endif