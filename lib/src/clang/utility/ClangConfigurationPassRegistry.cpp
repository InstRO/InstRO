#include "instro/instro/utility/ConfigurationLoader.h"

#include "instro/clang/core/PassFactory.h"

using namespace InstRO;
using namespace InstRO::Utility;

ClangConfigurationPassRegistry::ClangConfigurationPassRegistry(InstRO::Clang::PassFactory *factory)
	: BaseConfigurationPassRegistry(factory)
{
	// Selectors / Filters
	registerPass("BlackAndWhiteListSelector", [factory] (ConfigurationParsingContext &context) {
		context.expectInputPasses({0});
		return factory->createBlackAndWhiteListSelector(context.getStringArguments("blacklist"), context.getStringArguments("whitelist"));
	});
	registerPass("BooleanOrSelector", [factory] (ConfigurationParsingContext &context) {
		context.expectInputPasses({2});
		return factory->createBooleanOrSelector(context.inputPasses[0], context.inputPasses[1]);
	});
	registerPass("FunctionDefinitionSelector", [factory] (ConfigurationParsingContext &context) {
		context.expectInputPasses({0});
		return factory->createFunctionDefinitionSelector();
	});


	// Transformers


	// Adapters
	registerPass("CygProfileAdapter", [factory] (ConfigurationParsingContext &context) {
		context.expectInputPasses({1});
		return factory->createCygProfileAdapter(context.inputPasses[0]);
	});
	registerPass("LLVMInputAdapter", [factory] (ConfigurationParsingContext &context) {
		context.expectInputPasses({1});
		return factory->createLLVMInputAdapter(context.inputPasses[0]);
	});
}
