#include "rose.h" // including this first somehow prevents errors due to invalid string suffixes and turns them into warnings...
#include "instro/utility/ConfigurationLoader.h"
#include "instro/rose/RosePassFactory.h"
#include "instro/utility/exception.h"

using namespace InstRO;
using namespace InstRO::Utility;

RoseConfigurationPassRegistry::RoseConfigurationPassRegistry(InstRO::Rose::RosePassFactory *factory)
	: BaseConfigurationPassRegistry(factory)
{
	// TODO: Rose-prefixes?, parse ConstructTraitType
	
	// Selectors / Filters
	registerPass("ProgramEntrySelector", [factory] (ConfigurationParsingContext &context) {
		return factory->createProgramEntrySelector();
	});
	registerPass("IdentifyerSelector", [factory] (ConfigurationParsingContext &context) {
		return factory->createIdentifyerSelector(context.getStringArguments());
	});
	registerPass("IdentifyerFilter", [factory] (ConfigurationParsingContext &context) -> Pass* {
		if (context.inputPasses.size() == 1) {
			return factory->createIdentifyerFilter(context.getStringArguments(), context.inputPasses[0]);
		} else {
			InstRO::raise_exception("Expected one input pass, got " + std::to_string(context.inputPasses.size()));
		}
		
		return nullptr;
	});
	registerPass("TextStringSelector", [factory] (ConfigurationParsingContext &context) {
		return factory->createTextStringSelector(context.getStringArguments());
	});
	registerPass("CallPathSelector", [factory] (ConfigurationParsingContext &context) -> Pass* {
		if (context.inputPasses.size() == 2) {
			return factory->createCallPathSelector(context.inputPasses[0], context.inputPasses[1]);
		} else {
			InstRO::raise_exception("Expected two input passes, got " + std::to_string(context.inputPasses.size()));
		}
		
		return nullptr;
	});
	registerPass("BooleanOrSelector", [factory] (ConfigurationParsingContext &context) -> Pass* {
		if (context.inputPasses.size() == 2) {
			return factory->createBooleanOrSelector(context.inputPasses[0], context.inputPasses[1]);
		} else {
			InstRO::raise_exception("Expected two input passes, got " + std::to_string(context.inputPasses.size()));
		}
		
		return nullptr;
	});
	registerPass("FunctionBlackAndWhiteListSelector", [factory] (ConfigurationParsingContext &context) {
		return factory->createFunctionBlackAndWhiteListSelector(context.getStringArguments());
	});
/*	registerPass("ConstructLoweringElevator", [factory] (ConfigurationParsingContext &context) -> Pass* {
		if (context.inputPasses.size() == 1) {
			return factory->createConstructLoweringElevator(context.inputPasses[0]);
			return factory->createIdentifyerFilter(context.getStringArguments(), context.inputPasses[0]);
		} else {
			InstRO::raise_exception("Expected one input pass, got " + std::to_string(context.inputPasses.size()));
		}
		
		return nullptr;
	});*/
	
	
	
	// Transformers
	registerPass("UniqueCallpathTransformer", [factory] (ConfigurationParsingContext &context) -> Pass* {
		if (context.inputPasses.size() == 1) {
			return factory->createUniqueCallpathTransformer(context.inputPasses[0]);
		} else if (context.inputPasses.size() == 3) {
			return factory->createUniqueCallpathTransformer(context.inputPasses[0], context.inputPasses[1], context.inputPasses[2]);
		} else {
			InstRO::raise_exception("Expected one or three input passes for UniqueCallpathTransformer, got " + std::to_string(context.inputPasses.size()));
		}
		
		return nullptr;
	});
	
	
	// Adapters
	registerPass("ConstructPrinter", [factory] (ConfigurationParsingContext &context) -> Pass* {
		if (context.inputPasses.size() == 1) {
			return factory->createConstructPrinter(context.inputPasses[0]);
		} else {
			InstRO::raise_exception("Expected one input pass, got " + std::to_string(context.inputPasses.size()));
		}
		
		return nullptr;
	});
	registerPass("ConstructHierarchyASTDotGenerator", [factory] (ConfigurationParsingContext &context) -> Pass* {
		if (context.inputPasses.size() == 1) {
			return factory->createConstructHierarchyASTDotGenerator(context.inputPasses[0], context.getStringArgument("filename"));
		} else {
			InstRO::raise_exception("Expected one input pass, got " + std::to_string(context.inputPasses.size()));
		}
		
		return nullptr;
	});
	
	
	
	
}

