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
	InstRO::Pass *createTestAdapter(InstRO::Pass *input, std::set<std::string> expectedItems) {
		auto pImpl = new InstRO::Test::TestAdapter(input, expectedItems);
		InstRO::Pass *p = new InstRO::Pass(pImpl);
		p->setPassName("TestAdapter");
		passManager->registerPass(p);
		return p;
	}
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
