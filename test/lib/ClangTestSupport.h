#ifndef TEST_LIB_CLANGTESTSUPPORT_H
#define TEST_LIB_CLANGTESTSUPPORT_H

#include <regex>

#include "instro.h"
#include "lib/TestAdapter.h"

namespace ClangTest {

class ClangTestAdapter : public InstRO::Test::TestAdapter {
 public:
	ClangTestAdapter(std::string lab, std::string filename, InstRO::Test::TestSummary *tr)
			: InstRO::Test::TestAdapter(lab, filename, tr) {}

 protected:
	bool constructMatchesAnyExpectation(std::string &testIdentifier,
																			std::shared_ptr<InstRO::Core::Construct> construct) const override {
		if (getExpectedItems().find(testIdentifier) != getExpectedItems().end()) {
			return true;
		} else {
			// check whether the construct is a SimpleStatement shadowing an expression
			InstRO::Core::ConstructTrait constructTrait = construct->getTraits();
			if (constructTrait.is(InstRO::Core::ConstructTraitType::CTSimpleStatement) &&
					constructTrait.is(InstRO::Core::ConstructTraitType::CTExpression)) {
				std::regex positionPattern(".+:(\\d+):(\\d+)--(\\w+)");
				std::smatch positionMatch;
				if (!std::regex_match(testIdentifier, positionMatch, positionPattern)) {
					InstRO::logIt(InstRO::ERROR) << "Position pattern does not match the test identifier '" << testIdentifier
																			 << "'" << std::endl;
					return false;
				}
				if (positionMatch.size() != 4) {
					InstRO::logIt(InstRO::ERROR) << "Unexpected number of sub matches (" << positionMatch.size() << ") for "
																			 << construct->toString() << std::endl;
					return false;
				}
				int line = std::stoi(positionMatch[1].str());
				int column = std::stoi(positionMatch[2].str());
				for (auto item : getExpectedItems()) {
					if (!std::regex_match(item, positionMatch, positionPattern) || positionMatch.size() != 4) {
						InstRO::logIt(InstRO::WARN) << "Failed to match expected item '" << item << "'" << std::endl;
						continue;
					}
					int itemLine = std::stoi(positionMatch[1].str());
					int itemColumn = std::stoi(positionMatch[2].str());
					if (line == itemLine && column == itemColumn) {
						if (positionMatch[3].str() == "Expression") {
							InstRO::logIt(InstRO::DEBUG) << "'" << testIdentifier << "' matches item '" << item << "'" << std::endl;
							// change testIdentifier to the matched item
							testIdentifier = item;
							return true;
						}
					}
				}
			}
			return false;
		}
	}
};

class ClangTestInstrumentor;

/**
 * Extends the normal ClangPassFactory with the possibility to create a TestAdapter instance
 */
class ClangTestFactory : public InstRO::Clang::ClangPassFactory {
 public:
	ClangTestFactory(InstRO::PassManagement::PassManager *manager, clang::tooling::Replacements &reps)
			: InstRO::Clang::ClangPassFactory(manager, reps) {}

	InstRO::Pass *createTestAdapter(InstRO::Pass *input, std::string label, std::string filename) {
		std::unique_ptr<InstRO::Test::TestSummary> tr(new InstRO::Test::TestSummary(label));
		testSummaries.push_back(std::move(tr));

		auto pImpl = new ClangTest::ClangTestAdapter(label, filename, testSummaries.back().get());
		InstRO::Pass *p = new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(input), "TestAdapter");
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

	ClangTestFactory *getFactory(
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
