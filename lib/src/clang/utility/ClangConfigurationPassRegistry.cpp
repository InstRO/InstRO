#include "instro/utility/ConfigurationLoader.h"

#include "instro/clang/ClangPassFactory.h"

using namespace InstRO;
using namespace InstRO::Utility;

ClangConfigurationPassRegistry::ClangConfigurationPassRegistry(InstRO::Clang::ClangPassFactory *factory)
		: BaseConfigurationPassRegistry(factory) {
	// Selectors / Filters
	registerPass("BlackAndWhiteListSelector", [factory](ConfigurationParsingContext &context) {
		context.expectInputPasses({0});
		return factory->createClangBlackAndWhiteListSelector(context.getStringArguments("blacklist"),
																										context.getStringArguments("whitelist"));
	});

	// Transformers

	// Adapters
	registerPass("CygProfileAdapter", [factory](ConfigurationParsingContext &context) {
		context.expectInputPasses({1});
		return factory->createClangCygProfileAdapter(context.inputPasses[0]);
	});
	registerPass("ClangMangledNameOutputAdapter", [factory](ConfigurationParsingContext &context) {
		context.expectInputPasses({1});
		return factory->createClangMangledNameOutputAdapter(context.inputPasses[0]);
	});
}
