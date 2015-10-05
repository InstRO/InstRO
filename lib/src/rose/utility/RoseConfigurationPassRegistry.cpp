#include "rose.h" // including this first somehow prevents errors due to invalid string suffixes and turns them into warnings...
#include "instro/utility/ConfigurationLoader.h"
#include "instro/rose/RosePassFactory.h"
#include "instro/utility/exception.h"

using namespace InstRO;
using namespace InstRO::Utility;

RoseConfigurationPassRegistry::RoseConfigurationPassRegistry(InstRO::Rose::RosePassFactory *factory)
	: BaseConfigurationPassRegistry(factory)
{
	// Selectors / Filters
	registerPass("ProgramEntrySelector", [factory] (ConfigurationParsingContext &context) {
		return factory->createProgramEntrySelector();
	});
	registerPass("IdentifyerSelector", [factory] (ConfigurationParsingContext &context) {
		return factory->createIdentifyerSelector(context.getStringArguments());
	});
	registerPass("IdentifyerFilter", [factory] (ConfigurationParsingContext &context) -> Pass* {
		context.expectInputPasses({0,1});
		return factory->createIdentifyerFilter(context.getStringArguments(), context.inputPasses[0]);
	});
	registerPass("CallPathSelector", [factory] (ConfigurationParsingContext &context) -> Pass* {
		context.expectInputPasses({2});
		return factory->createCallPathSelector(context.inputPasses[0], context.inputPasses[1]);
	});
	registerPass("BooleanOrSelector", [factory] (ConfigurationParsingContext &context) -> Pass* {
		context.expectInputPasses({1});
		return factory->createBooleanOrSelector(context.inputPasses[0], context.inputPasses[1]);
	});
	registerPass("FunctionBlackAndWhiteListSelector", [factory] (ConfigurationParsingContext &context) {
		return factory->createFunctionBlackAndWhiteListSelector(context.getStringArguments());
	});


	// Transformers
	registerPass("ConstructLoweringElevator", [factory] (ConfigurationParsingContext &context) -> Pass* {
		context.expectInputPasses({1});
		return factory->createConstructLoweringElevator(context.inputPasses[0], context.getConstructTraitType("level"));
	});
	registerPass("ConstructRaisingElevator", [factory] (ConfigurationParsingContext &context) -> Pass* {
		context.expectInputPasses({1});
		return factory->createConstructRaisingElevator(context.inputPasses[0], context.getConstructTraitType("level"));
	});
	registerPass("UniqueCallpathTransformer", [factory] (ConfigurationParsingContext &context) -> Pass* {
		context.expectInputPasses({1,3});
		if (context.inputPasses.size() == 1) {
			return factory->createUniqueCallpathTransformer(context.inputPasses[0]);
		} else if (context.inputPasses.size() == 3) {
			return factory->createUniqueCallpathTransformer(context.inputPasses[0], context.inputPasses[1], context.inputPasses[2]);
		}

		return nullptr;
	});


	// Adapters
	registerPass("ConstructPrinter", [factory] (ConfigurationParsingContext &context) -> Pass* {
		context.expectInputPasses({1});
		return factory->createConstructPrinter(context.inputPasses[0]);

	});
	registerPass("ConstructHierarchyASTDotGenerator", [factory] (ConfigurationParsingContext &context) -> Pass* {
		context.expectInputPasses({1});
		return factory->createConstructHierarchyASTDotGenerator(context.inputPasses[0], context.getStringArgument("filename"));
	});




}

