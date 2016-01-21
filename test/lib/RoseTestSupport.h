#ifndef TEST_LIB_ROSETESTSUPPORT_H
#define TEST_LIB_ROSETESTSUPPORT_H

#include "instro.h"
#include "lib/TestAdapter.h"

namespace RoseTest {
class RoseTestInstrumentor;
/**
 * Extends the normal RosePassFactory with the possibility to create a TestAdapter instance
 */
class RoseTestFactory : public InstRO::Rose::RosePassFactory {
 public:
	RoseTestFactory(InstRO::PassManagement::PassManager *manager, SgProject *project)
			: InstRO::Rose::RosePassFactory(manager, project) {}

	InstRO::Pass *createTestAdapter(InstRO::Pass *input, std::string label, std::string filename) {
		std::unique_ptr<InstRO::Test::TestSummary> tr(new InstRO::Test::TestSummary(label));
		testSummaries.push_back(std::move(tr));

		auto pImpl = new InstRO::Test::TestAdapter(label, filename, testSummaries.back().get());
		InstRO::Pass *p = new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(input), "TestAdapter");
//		p->setPassName("TestAdapter " + label);
		passManager->registerPass(p);
		return p;
	}

 private:
	friend class RoseTestInstrumentor;
	std::vector<std::unique_ptr<InstRO::Test::TestSummary>> testSummaries;
};

/**
 * Gives acces to the RoseTestFactory, capable to instantiate a TestAdapter object
 */
class RoseTestInstrumentor : public InstRO::RoseInstrumentor {
 public:
	RoseTestInstrumentor(int argc, char **argv) : InstRO::RoseInstrumentor(argc, argv) {}

	RoseTestFactory *getFactory(
			InstRO::Instrumentor::CompilationPhase phase = InstRO::Instrumentor::CompilationPhase::frontend) override {
		if (fac == nullptr) {
			fac.reset(new RoseTestFactory(passManager, project));
		}
		return fac.get();
	}

	bool testFailed() {
		bool hasTestFailed(false);

		for (std::unique_ptr<InstRO::Test::TestSummary> &tr : fac->testSummaries) {
			tr->printResults();
			if (tr->failed()) {
				hasTestFailed = true;
			}
		}

		return hasTestFailed;
	}

 private:
	std::unique_ptr<RoseTestFactory> fac;
};
}

#endif
