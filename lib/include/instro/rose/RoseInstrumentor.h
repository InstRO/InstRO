#ifndef INSTRO_ROSE_INSTRUMENTOR_H
#define INSTRO_ROSE_INSTRUMENTOR_H

#include "instro/core/Instrumentor.h"
#include "instro/rose/RosePassFactory.h"
#include "instro/rose/tooling/RoseAnalysisInterface.h"

#include "instro/core/Singleton.h"

#include "rose.h"

namespace InstRO {
namespace Rose {
class RosePassFactory;
}

class RoseInstrumentor : public Instrumentor {
 public:
	RoseInstrumentor(int argc, char** argv) {
		project = ::frontend(argc, argv);
		ram = new InstRO::Rose::Tooling::RoseAnalysisManager(project);
		InstRO::setInstrumentorInstance(this);
	};

	~RoseInstrumentor() { delete (project); }

	virtual Rose::RosePassFactory* getFactory(
			Instrumentor::CompilationPhase phase = Instrumentor::CompilationPhase::frontend) override {
		return new Rose::RosePassFactory(passManager, project);
	}
	void init() {}
	void apply() { passManager->execute(); };
	void finalize() {
		// unparse instrumented source
		project->unparse();
		project->compileOutput();
	};

	virtual Tooling::AnalysisManager* getAnalysisManager() { return ram; }

 protected:
	// Store the Project. It is required for all passes later on ..
	SgProject* project;
	InstRO::Rose::Tooling::RoseAnalysisManager* ram;
};
};
#endif
