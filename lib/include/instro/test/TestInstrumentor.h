#ifndef INSTRO_ROSE_INSTRUMENTOR_H
#define INSTRO_ROSE_INSTRUMENTOR_H

#include "instro/core/Instrumentor.h"
// #include "instro/core/PassFactory.h"

#include "instro/test/TestPassFactory.h"

namespace InstRO
{
	namespace Test{
	class TestPassFactory;
	}
	class TestInstrumentor: public Instrumentor
	{
	public: 
		Test::TestPassFactory * getFactory(Instrumentor::CompilationPhase phase){ return new Test::TestPassFactory(passManager); }
		// TestPassFactory * getFactory(Instrumentor::CompilationPhase phase){return dynamic_cast<PassFactory*>(new Test::TestPassFactory(passManager));}
//		PassFactory * getFactory(In
		void init(){};
		void apply(){};
		void finalize(){};
	};
	
};
#endif