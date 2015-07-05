#ifndef INSTRO_ROSE_INSTRUMENTOR_H
#define INSTRO_ROSE_INSTRUMENTOR_H

#include "instro/core/Instrumentor.h"
#include "instro/rose/RosePassFactory.h"

#include "rose.h"

namespace InstRO {
namespace Rose {
// Forward Declaration of the RosePassFactory
class RosePassFactory;
}

class RoseInstrumentor : public Instrumentor {
 protected:
	// Store the Project. It is required for all passes later on ..
	SgProject* project;

 public:
	RoseInstrumentor(int argc, char** argv) {
		// TODO: Initialize Rose here
		project = ::frontend(argc, argv);
	};
	~RoseInstrumentor() { delete (project); }
	InstRO::PassFactory* getFactory(Instrumentor::CompilationPhase phase) {
		//		lockPassManager();
		return new Rose::RosePassFactory(passManager, project);
	}
	//		PassFactory * getFactory(In
	void init(){

	};
	void apply(){};
	void finalize() {
		// unparse instrumented source
		project->unparse();
	};

	virtual Tooling::AnalysisManager* getAnalysisManager() {
		throw std::string("Not Implemented");
		return NULL;
	}
};
};
#endif
