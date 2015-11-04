#include "rose.h"	// including this first somehow prevents errors due to invalid string suffixes and turns them into warnings...
#include "instro/utility/ConfigurationLoader.h"
#include "instro/rose/RosePassFactory.h"
#include "instro/utility/exception.h"

using namespace InstRO;
using namespace InstRO::Utility;

// TODO RN 2015-11: which factory methods are still missing here?
RoseConfigurationPassRegistry::RoseConfigurationPassRegistry(InstRO::Rose::RosePassFactory *factory)
		: BaseConfigurationPassRegistry(factory) {
	// Selectors / Filters
	registerPass("ProgramEntrySelector",
							 [factory](ConfigurationParsingContext &context) { return factory->createProgramEntrySelector(); });
	registerPass("IdentifyerSelector", [factory](ConfigurationParsingContext &context) {
		return factory->createIdentifierMatcherSelector(context.getStringArguments());
	});
	registerPass("CallPathSelector", [factory](ConfigurationParsingContext &context) -> Pass *{
		context.expectInputPasses({2});
		return factory->createCallpathSelector(context.inputPasses[0], context.inputPasses[1]);
	});
	registerPass("BooleanOrSelector", [factory](ConfigurationParsingContext &context) -> Pass *{
		context.expectInputPasses({1});
		return factory->createBooleanOrSelector(context.inputPasses[0], context.inputPasses[1]);
	});

	// Transformers
	registerPass("ConstructLoweringElevator", [factory](ConfigurationParsingContext &context) -> Pass *{
		context.expectInputPasses({1});
		return factory->createConstructLoweringElevator(context.inputPasses[0], context.getConstructTraitType("level"));
	});
	registerPass("ConstructRaisingElevator", [factory](ConfigurationParsingContext &context) -> Pass *{
		context.expectInputPasses({1});
		return factory->createConstructRaisingElevator(context.inputPasses[0], context.getConstructTraitType("level"));
	});
	registerPass("UniqueCallpathTransformer", [factory](ConfigurationParsingContext &context) -> Pass *{
		context.expectInputPasses({1, 3});
		if (context.inputPasses.size() == 1) {
			return factory->createUniqueCallpathTransformer(context.inputPasses[0]);
		} else if (context.inputPasses.size() == 3) {
			return factory->createUniqueCallpathTransformer(context.inputPasses[0], context.inputPasses[1],
																											context.inputPasses[2]);
		}

		return nullptr;
	});
	registerPass("MPIFunctionWrapper", [factory](ConfigurationParsingContext &context) -> Pass *{
		context.expectInputPasses({1, 2});
		InstRO::Pass *renamingPass = (context.inputPasses.size() == 2) ? context.inputPasses[1] : nullptr;

		// prefixes are optional and irrelevant for most use cases
		std::string definitionPrefix = context.getStringArgumentOrDefault("definitionPrefix", std::string());
		std::string wrapperPrefix = context.getStringArgumentOrDefault("wrapperPrefix", std::string());

		return factory->createMPIFunctionWrapper(context.inputPasses[0], renamingPass, definitionPrefix, wrapperPrefix);
	});

	// Adapters
	registerPass("ConstructPrinter", [factory](ConfigurationParsingContext &context) -> Pass *{
		context.expectInputPasses({1});
		return factory->createConstructPrinter(context.inputPasses[0]);

	});
	registerPass("ConstructHierarchyASTDotGenerator", [factory](ConfigurationParsingContext &context) -> Pass *{
		context.expectInputPasses({1});
		return factory->createConstructHierarchyASTDotGenerator(context.inputPasses[0],
																														context.getStringArgument("filename"));
	});
}
