#ifndef TEST_LIB_ROSETESTSUPPORT_H
#define TEST_LIB_ROSETESTSUPPORT_H

#include "instro.h"
#include "lib/TestAdapter.h"

namespace RoseTest {

/**
 * Extends the normal RosePassFactory with the possibility to create a TestAdapter instance
 */
class RoseTestFactory : public InstRO::Rose::RosePassFactory {
 public:
	RoseTestFactory(InstRO::PassManagement::PassManager *manager, SgProject *project)
			: InstRO::Rose::RosePassFactory(manager, project) {}

	InstRO::Pass *createTestAdapter(InstRO::Pass *input, std::string label, std::string filename) {
		std::unique_ptr<InstRO::Test::TestReporter> tr(new InstRO::Test::TestReporter(label));
		testReporter.push_back(std::move(tr));

		auto pImpl = new InstRO::Test::TestAdapter(input, label, filename, testReporter.back().get());
		InstRO::Pass *p = new InstRO::Pass(pImpl);
		p->setPassName("TestAdapter " + label);
		passManager->registerPass(p);
		return p;
	}

	std::vector<std::unique_ptr<InstRO::Test::TestReporter>> testReporter;
};

/**
 * Gives acces to the RoseTestFactory, capable to instantiate a TestAdapter object
 */
class RoseTestInstrumentor : public InstRO::RoseInstrumentor {
 public:
	RoseTestInstrumentor(int argc, char **argv) : InstRO::RoseInstrumentor(argc, argv) {}

	RoseTestFactory *getFactory(
			InstRO::Instrumentor::CompilationPhase phase = InstRO::Instrumentor::CompilationPhase::frontend) override {
		return new RoseTestFactory(passManager, project);
	}
};
}

#endif
