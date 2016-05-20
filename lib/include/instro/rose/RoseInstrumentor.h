#ifndef INSTRO_ROSE_INSTRUMENTOR_H
#define INSTRO_ROSE_INSTRUMENTOR_H

#include "instro/Instrumentor.h"
#include "instro/rose/RosePassFactory.h"
#include "instro/rose/tooling/RoseAnalysisInterface.h"

#include "instro/core/Singleton.h"

#include "instro/utility/CommandlineHandling.h"

#include "rose.h"

namespace InstRO {
namespace Rose {
class RosePassFactory;
}

class RoseInstrumentor : public Instrumentor {
 public:
	RoseInstrumentor(int argc, char **argv) : passFactory(nullptr) {
		InstRO::Utility::RoseCLIPreparation prepStrat(&argc, &argv);
		std::vector<std::string> argVec = prepStrat.getCommandLine();

		project = ::frontend(argVec);

		ram = new InstRO::Rose::Tooling::RoseAnalysisManager(project);
		InstRO::setInstrumentorInstance(this);
	}

	~RoseInstrumentor() {
		delete project;
		delete passFactory;
		delete ram;
	}

	virtual Rose::RosePassFactory *getFactory(
			Instrumentor::CompilationPhase phase = Instrumentor::CompilationPhase::frontend) override {
		if (!passFactory) {
			passFactory = new Rose::RosePassFactory(passManager, project);
		}

		return passFactory;
	}

	void apply() override {
		passManager->execute();
		project->unparse();
		if (!project->get_skipfinalCompileStep()) {
			int err = project->compileOutput();
			if (err) {
				throw std::string("There was an error compiling the unparsed sources");
			}
		}
	}

	virtual Tooling::AnalysisManager *getAnalysisManager() override { return ram; }

 protected:
	// Store the Project. It is required for all passes later on ..
	SgProject *project;
	InstRO::Rose::Tooling::RoseAnalysisManager *ram;
	Rose::RosePassFactory *passFactory;
};
}
#endif
