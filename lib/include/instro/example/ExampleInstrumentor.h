#ifndef INSTRO_ROSE_INSTRUMENTOR_H
#define INSTRO_ROSE_INSTRUMENTOR_H

#include "instro/core/Instrumentor.h"
// #include "instro/core/PassFactory.h"

#include "instro/example/ExamplePassFactory.h"
#include "instro/example/ExampleAnalysisInterface.h"

namespace InstRO
{
	namespace Example{
		class ExamplePassFactory;
	}
	class ExampleInstrumentor : public Instrumentor
	{
	protected:
		Example::Tooling::ExampleAnalysisManager * analysis_manager;
	public: 
		ExampleInstrumentor() :analysis_manager(new Example::Tooling::ExampleAnalysisManager()){};

		Example::ExamplePassFactory * getFactory(Instrumentor::CompilationPhase phase){ return new Example::ExamplePassFactory(passManager); }
		// TestPassFactory * getFactory(Instrumentor::CompilationPhase phase){return dynamic_cast<PassFactory*>(new Test::TestPassFactory(passManager));}
//		PassFactory * getFactory(In
		void init(){};
		void apply(){};
		void finalize(){};

		// Interface to access the implementation specific Analysis Layer Container
		virtual Tooling::AnalysisManager * getAnalysisManager() {
			return analysis_manager;
		}
	};
	
};
#endif