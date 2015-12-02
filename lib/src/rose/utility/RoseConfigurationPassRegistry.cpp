#include "rose.h"	// including this first somehow prevents errors due to invalid string suffixes and turns them into warnings...
#include "instro/utility/ConfigurationLoader.h"
#include "instro/rose/RosePassFactory.h"
#include "instro/utility/exception.h"

using namespace InstRO;
using namespace InstRO::Utility;

RoseConfigurationPassRegistry::RoseConfigurationPassRegistry(InstRO::Rose::RosePassFactory *factory)
		: BaseConfigurationPassRegistry(factory) {
	// Transformers
	registerPass("RoseUniqueCallpathTransformer", [factory](ConfigurationParsingContext &context) -> Pass * {
		context.expectInputPasses({1, 3});
		if (context.inputPasses.size() == 1) {
			return factory->createRoseUniqueCallpathTransformer(context.inputPasses[0]);
		} else if (context.inputPasses.size() == 3) {
			return factory->createRoseUniqueCallpathTransformer(context.inputPasses[0], context.inputPasses[1],
																											context.inputPasses[2]);
		}
		return nullptr;
	});

	registerPass("RoseMPIFunctionWrapper", [factory](ConfigurationParsingContext &context) -> Pass * {
		context.expectInputPasses({1, 2});
		InstRO::Pass *renamingPass = (context.inputPasses.size() == 2) ? context.inputPasses[1] : nullptr;

		// prefixes are optional and irrelevant for most use cases
		std::string definitionPrefix = context.getStringArgumentOrDefault("definitionPrefix", std::string());
		std::string wrapperPrefix = context.getStringArgumentOrDefault("wrapperPrefix", std::string());

		return factory->createRoseMPIFunctionWrapper(context.inputPasses[0], renamingPass, definitionPrefix, wrapperPrefix);
	});
}
