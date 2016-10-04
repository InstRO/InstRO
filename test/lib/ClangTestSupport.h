#ifndef TEST_LIB_CLANGTESTSUPPORT_H
#define TEST_LIB_CLANGTESTSUPPORT_H

#include "instro.h"
#include "instro/clang/core/ClangConstruct.h"
#include "lib/TestAdapter.h"

#include <sstream>
#include <regex>


namespace ClangTest {

class ClangTestAdapter : public InstRO::Test::TestAdapter {
 public:
	ClangTestAdapter(std::string lab, std::string filename, InstRO::Test::TestSummary *tr,
									 std::multiset<std::string> &initializerExpressions)
			: InstRO::Test::TestAdapter(lab, filename, tr), initializerExpressions(initializerExpressions) {}

 protected:
	std::vector<std::string> constructMatchesAnyExpectation(
			const std::string &testIdentifier, std::shared_ptr<InstRO::Core::Construct> construct) const override {
		std::vector<std::string> matchedIdentifiers;
		if (getExpectedItems().find(testIdentifier) != getExpectedItems().end()) {
			matchedIdentifiers.push_back(testIdentifier);

			// if the construct is a variable declaration, create an identifier for the initializer (Clang does not have a
			// separate AST node for that)
			std::shared_ptr<InstRO::Clang::Core::ClangConstruct> clangConstruct =
					std::dynamic_pointer_cast<InstRO::Clang::Core::ClangConstruct>(construct);
			if (clang::Decl *decl = clangConstruct->getAsDecl()) {
				if (clang::VarDecl *varDecl = llvm::dyn_cast<clang::VarDecl>(decl)) {
					createInitializerExpressionIdentifier(testIdentifier, varDecl);
				}
			}
		} else {
			// check whether the construct is a SimpleStatement overshadowing an expression
			InstRO::Core::ConstructTrait constructTrait = construct->getTraits();
			if (constructTrait.is(InstRO::Core::ConstructTraitType::CTSimpleStatement) &&
					constructTrait.is(InstRO::Core::ConstructTraitType::CTExpression)) {
				matchOvershadowedExpression(testIdentifier, matchedIdentifiers);
			}
		}
		return matchedIdentifiers;
	}

 private:
	std::multiset<std::string> &initializerExpressions;

	void createInitializerExpressionIdentifier(const std::string &testIdentifier, clang::VarDecl *varDecl) const {
		std::regex identifierPrefixPattern("(.+:)\\d+:\\d+--\\w+");
		std::smatch prefixMatch;
		if (!std::regex_match(testIdentifier, prefixMatch, identifierPrefixPattern)) {
			InstRO::logIt(InstRO::ERROR) << "Identifier prefix pattern does not match the test identifier '" << testIdentifier
																	 << "'" << std::endl;
			return;
		}
		if (prefixMatch.size() != 2) {
			InstRO::logIt(InstRO::ERROR) << "Unexpected number of sub matches (" << prefixMatch.size() << ") for '"
																	 << testIdentifier << "'" << std::endl;
			return;
		}
		std::ostringstream initializerIdentifier;
		initializerIdentifier << prefixMatch[1].str();

		// find location of the variable name in the declaration
		const clang::SourceManager &srcMgr = InstRO::Clang::Core::ClangConstruct::getSourceManager();
		clang::SourceLocation nameLocStart = varDecl->getLocation();
		unsigned nameLine = srcMgr.getSpellingLineNumber(nameLocStart);
		unsigned nameColumn = srcMgr.getSpellingColumnNumber(nameLocStart);
		initializerIdentifier << nameLine << ":" << nameColumn << "--Expression";
		InstRO::logIt(InstRO::DEBUG) << "Created identifier for variable assign initializer: "
																 << initializerIdentifier.str() << std::endl;
		initializerExpressions.insert(initializerIdentifier.str());
	}

	void matchOvershadowedExpression(const std::string &testIdentifier,
																	 std::vector<std::string> &matchedIdentifiers) const {
		std::regex positionPattern(".+:(\\d+):(\\d+)--(\\w+)");
		std::smatch positionMatch;
		if (!std::regex_match(testIdentifier, positionMatch, positionPattern)) {
			InstRO::logIt(InstRO::ERROR) << "Position pattern does not match the test identifier '" << testIdentifier << "'"
																	 << std::endl;
			return;
		}
		if (positionMatch.size() != 4) {
			InstRO::logIt(InstRO::ERROR) << "Unexpected number of sub matches (" << positionMatch.size() << ") for '"
																	 << testIdentifier << "'" << std::endl;
			return;
		}
		unsigned line = std::stoi(positionMatch[1].str());
		unsigned column = std::stoi(positionMatch[2].str());
		for (auto item : getExpectedItems()) {
			if (!std::regex_match(item, positionMatch, positionPattern) || positionMatch.size() != 4) {
				InstRO::logIt(InstRO::WARN) << "Failed to match expected item '" << item << "'" << std::endl;
				continue;
			}
			unsigned itemLine = std::stoi(positionMatch[1].str());
			unsigned itemColumn = std::stoi(positionMatch[2].str());
			if (line == itemLine && column == itemColumn) {
				if (positionMatch[3].str() == "Expression") {
					InstRO::logIt(InstRO::DEBUG) << "'" << testIdentifier << "' matches item '" << item << "'" << std::endl;
					matchedIdentifiers.push_back(item);
					break;
				}
			}
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

		auto pImpl = new ClangTest::ClangTestAdapter(label, filename, testSummaries.back().get(), initializerExpressions);
		InstRO::Pass *p = new InstRO::Pass(pImpl, InstRO::Core::ChannelConfiguration(input), "TestAdapter");
		passManager->registerPass(p);
		return p;
	}

 private:
	friend class ClangTestInstrumentor;
	std::vector<std::unique_ptr<InstRO::Test::TestSummary>> testSummaries;
	std::multiset<std::string> initializerExpressions;
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
			// forward initializer expressions generated by variable declarations to the expression summary
			if (tr->getLabel().find("Expression") != std::string::npos) {
				tr->assumeFound(fac->initializerExpressions);
			}
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
