#ifndef INSTRO_ROSE_INSTRUMENTOR_H
#define INSTRO_ROSE_INSTRUMENTOR_H

#include "instro/core/Instrumentor.h"
// #include "instro/core/PassFactory.h"

#include "instro/example/ExamplePassFactory.h"

namespace InstRO
{
	namespace Example{
		class ExamplePassFactory;
	}
	class ExampleInstrumentor : public Instrumentor
	{
	public: 
		Example::ExamplePassFactory * getFactory(Instrumentor::CompilationPhase phase){ return new Example::ExamplePassFactory(passManager); }
		// TestPassFactory * getFactory(Instrumentor::CompilationPhase phase){return dynamic_cast<PassFactory*>(new Test::TestPassFactory(passManager));}
//		PassFactory * getFactory(In
		void init(){};
		void apply(){};
		void finalize(){};
	};
	
};
#endif