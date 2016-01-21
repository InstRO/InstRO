#ifndef TEST_LIB_CLANGTESTSUPPORT_H
#define TEST_LIB_CLANGTESTSUPPORT_H

#include "instro.h"
#include "lib/TestAdapter.h"

namespace ClangTest {

class ClangTestInstrumentor;

/**
 * Extends the normal ClangPassFactory with the possibility to create a TestAdapter instance
 */
class ClangTestFactory : public InstRO::Clang::ClangPassFactory {
 public:
	ClangTestFactory(InstRO::PassManagement::PassManager *manager, clang::tooling::Replacements& reps)
			: InstRO::Clang::ClangPassFactory(manager, reps) {}

	InstRO::Pass *createTestAdapter(InstRO::Pass *input, std::string label, std::string filename) {
		std::unique_ptr<InstRO::Test::TestSummary> tr(new InstRO::Test::TestSummary(label));
		testSummaries.push_back(std::move(tr));

		auto pImpl = new InstRO::Test::TestAdapter(input, label, filename, testSummaries.back().get());
		InstRO::Pass *p = new InstRO::Pass(pImpl);
		p->setPassName("TestAdapter " + label);
		passManager->registerPass(p);
		return p;
	}

 private:
	friend class ClangTestInstrumentor;
	std::vector<std::unique_ptr<InstRO::Test::TestSummary>> testSummaries;
};

/**
 * Gives acces to the ClangTestFactory, capable to instantiate a TestAdapter object
 */
class ClangTestInstrumentor : public InstRO::Clang::ClangInstrumentor {
 public:
	ClangTestInstrumentor(int argc, char **argv, llvm::cl::OptionCategory &llvmThing)
			: InstRO::Clang::ClangInstrumentor(argc, const_cast<const char **>(argv), llvmThing) {}

	ClangTestFactory* getFactory (
			InstRO::Instrumentor::CompilationPhase phase = InstRO::Instrumentor::CompilationPhase::frontend) override {
		if (fac == nullptr) {
			fac.reset(new ClangTestFactory(passManager, getTool().getReplacements()));
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
	std::unique_ptr<ClangTestFactory> fac;
};
}

#endif
