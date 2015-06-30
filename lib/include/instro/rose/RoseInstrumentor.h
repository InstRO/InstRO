#ifndef INSTRO_ROSE_INSTRUMENTOR_H
#define INSTRO_ROSE_INSTRUMENTOR_H

#include "instro/core/Instrumentor.h"
// #include "instro/core/PassFactory.h"

#include "instro/rose/RosePassFactory.h"

namespace InstRO
{
	namespace Rose{
// Forward Declaration of the RosePassFactory
class RosePassFactory;
}
	
class RoseInstrumentor: public Instrumentor{
public: 
	RoseInstrumentor(int * argc,char *** argv){
			// TODO: Initialize Rose here
	};
	InstRO::PassFactory * getFactory(Instrumentor::CompilationPhase phase) {
//		lockPassManager();
			return new Rose::RosePassFactory(passManager);
	}
//		PassFactory * getFactory(In
	void init(){};
	void apply(){};
	void finalize(){};

	virtual Tooling::AnalysisManager * getAnalysisManager()	{
		throw std::string("Not Implemented");
		return NULL;
	}
	};

	
};
#endif