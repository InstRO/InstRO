#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface

#include "instro/core/ConstructSet.h"
#include "instro/tooling/NamedConstructAccess.h"
#include "instro/example/ExampleConstructSet.h"

namespace InstRO {
namespace Example {
namespace Tooling {
namespace NamedConstructAccess {

class ExampleNamedConstructAccess : public ::InstRO::Tooling::NamedConstructAccess::NamedConstructAccess {
 protected:
	std::set<std::string> namedConstructs;
	std::set<std::string> userTextStrings;

 public:
	ExampleNamedConstructAccess() {
		namedConstructs.insert("main");
		namedConstructs.insert("foo");
		namedConstructs.insert("myGoo");
		userTextStrings.insert("Hello World\n");
	};

	std::unique_ptr<InstRO::Core::ConstructSet> getConstructsByIdentifyerName(
			::InstRO::Tooling::NamedConstructAccess::Matcher &matcher) override {
		InstRO::Core::ConstructSet *cs = new InstRO::Core::ConstructSet();
		InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci(cs);
		for (auto identifyer : namedConstructs) {
			if (matcher.isMatch(identifyer))
				csci.put(std::make_shared<InstRO::Example::Core::ExampleConstruct>(
						InstRO::Example::Core::ExampleConstruct(InstRO::Core::ConstructTraitType::CTFunction, identifyer)));
		}
		return std::unique_ptr<InstRO::Core::ConstructSet>(cs);
	};
	std::unique_ptr<InstRO::Core::ConstructSet> getConstructsByUserTextStringMatch(
			::InstRO::Tooling::NamedConstructAccess::Matcher &matcher) override {
		InstRO::Core::ConstructSet *cs = new InstRO::Core::ConstructSet();
		InstRO::InfrastructureInterface::ConstructSetCompilerInterface csci(cs);
		for (auto identifyer : userTextStrings) {
			if (matcher.isMatch(identifyer))
				csci.put(std::make_shared<InstRO::Example::Core::ExampleConstruct>(
						InstRO::Example::Core::ExampleConstruct(InstRO::Core::ConstructTraitType::CTExpression, identifyer)));
		}
		return std::unique_ptr<InstRO::Core::ConstructSet>(cs);
	};
};
}
}
}
}
