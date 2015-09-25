#include "instro/utility/ConfigurationLoader.h"

#include "instro/utility/exception.h"

#include "rapidjson/filereadstream.h"
#include "rapidjson/error/en.h"

#include <type_traits> //std::underlying_type

using namespace InstRO;
using namespace InstRO::Utility;


// ConfigurationLoader

ConfigurationLoader::ConfigurationLoader(std::unique_ptr<ConfigurationPassRegistry> passRegistry)
	: passRegistry(std::move(passRegistry)) {}

ConfigurationLoader::PassMap ConfigurationLoader::getPasses() const {
	return passes;
}

Pass* ConfigurationLoader::getPass(const std::string &passId) const {
	auto passIter = passes.find(passId);

	if (passIter != passes.end()) {
		return passIter->second;
	} else {
		return nullptr;
	}
}

void ConfigurationLoader::load(const std::string &filename) {
	ConfigurationParser parser (*this);
	parser.parseFile(filename);
}


// ConfigurationParser

void ConfigurationParser::parseFile(const std::string &filename) {
	FILE *file = std::fopen(filename.c_str(), "r");
	if (!file) {
		std::cerr << "Failed to open file '" << filename << "'" << std::endl;
		return;
	}

	// parse JSON file into DOM
	char buffer[1024];
	rapidjson::FileReadStream readStream (file, buffer, sizeof(buffer));
	doc.ParseStream(readStream);

	std::fclose(file);

	if (doc.HasParseError()) {
		std::cerr << "An error occurred while parsing the JSON document at position " << doc.GetErrorOffset() << ": "
			<<  rapidjson::GetParseError_En(doc.GetParseError()) << std::endl;
	}

	// parse the individual passes which are specified inside the top level array
	for (auto passValueIter = doc.Begin(); passValueIter != doc.End(); ++passValueIter) {
		parsePass(*passValueIter);
	}
}

Pass* ConfigurationParser::parsePass(rapidjson::Value &passValue) {
	std::string passId = passValue["id"].GetString();

	// check whether the pass has already been parsed (due to a forward dependency)
	if (Pass *pass = loader.getPass(passId)) {
		return pass;
	}

	// create context: ensure that input passes are already parsed
	ConfigurationParsingContext context (passValue, getInputPasses(passValue));

	std::string passType = passValue["type"].GetString();

	// retrieve and invoke a parser for the specified type from the registry
	Pass *pass = nullptr;
	auto passParser = loader.passRegistry->lookup(passType);
	if (passParser) {
		pass = passParser(context);
		loader.passes.insert(std::make_pair(passId, pass));
	} else {
		InstRO::raise_exception(passId + ": Failed to find a parser for a pass of type '" + passType + "'");
	}

	return pass;
}

rapidjson::Value* ConfigurationParser::findPassValue(const std::string &passId) {
	for (auto passValueIter = doc.Begin(); passValueIter != doc.End(); ++passValueIter) {
		std::string currentPassId = (*passValueIter)["id"].GetString();
		if (currentPassId == passId) {
			return passValueIter;
		}
	}

	return nullptr;
}

std::vector<Pass*> ConfigurationParser::getInputPasses(rapidjson::Value &passValue) {
	std::vector<Pass*> inputPasses;
	auto inputs = passValue.FindMember("inputs");

	// check whether any passes have been specified because 'inputs' is optional
	if (inputs != passValue.MemberEnd()) {
		rapidjson::Value &inputsValue = inputs->value;
		std::string passId = passValue["id"].GetString();
		if (!inputsValue.IsArray()) {
			InstRO::raise_exception(passId + ": 'inputs' must be an array");
		}

		inputPasses.reserve(inputsValue.Size());
		for (auto inputValueIter = inputsValue.Begin(); inputValueIter != inputsValue.End(); ++inputValueIter) {
			std::string inputId = inputValueIter->GetString();
			Pass *inputPass = loader.getPass(inputId);
			if (!inputPass) {
				// pass has not been parsed yet (id used before declaration)
				rapidjson::Value *inputPassValue = findPassValue(inputId);
				if (inputPassValue) {
					inputPass = parsePass(*inputPassValue);
				} else {
					// failed to find the declaration of that id
					InstRO::raise_exception(passId + ": No pass with id '" + inputId + "' has been declared");
				}
			}

			inputPasses.push_back(inputPass);
		}
	}

	return inputPasses;
}


// ConfigurationParsingContext

std::string ConfigurationParsingContext::getId() const {
	return passValue["id"].GetString();
}

std::string ConfigurationParsingContext::getType() const {
	return passValue["type"].GetString();
}

std::string ConfigurationParsingContext::getStringArgument(const char* memberName) const {
	auto memberIter = passValue.FindMember(memberName);

	if (memberIter != passValue.MemberEnd()) {
		return memberIter->value.GetString();
	} else {
		InstRO::raise_exception(getId() + ": Cannot find member '" + std::string(memberName) + "'");
		return std::string();
	}
}

std::vector<std::string> ConfigurationParsingContext::getStringArguments(const char* memberName) const {
	std::vector<std::string> arguments;
	auto memberIter = passValue.FindMember(memberName);

	if (memberIter != passValue.MemberEnd()) {
		rapidjson::Value &argumentsValue = memberIter->value;
		if (!argumentsValue.IsArray()) {
			InstRO::raise_exception(getId() + ": Member '" + std::string(memberName) + "' must be an array");
		}

		arguments.reserve(argumentsValue.Size());
		for (auto argValueIter = argumentsValue.Begin(); argValueIter != argumentsValue.End(); ++argValueIter) {
			arguments.push_back(argValueIter->GetString());
		}
	} else {
		InstRO::raise_exception(getId() + ": Cannot find member '" + std::string(memberName) + "'");
	}

	return arguments;
}

InstRO::Core::ConstructTraitType ConfigurationParsingContext::getConstructTraitType(const char* memberName) const {
	auto memberIter = passValue.FindMember(memberName);

	if (memberIter != passValue.MemberEnd()) {
		rapidjson::Value &cttValue = memberIter->value;
		if (cttValue.IsInt()) {
			auto ctt = cttValue.GetInt();
			// check whether the specified integer maps to a valid enumeration entry - must be in [CTMin, CTMax]
			auto minIntVal = static_cast<std::underlying_type<InstRO::Core::ConstructTraitType>::type>(InstRO::Core::ConstructTraitType::CTMin);
			auto maxIntVal = static_cast<std::underlying_type<InstRO::Core::ConstructTraitType>::type>(InstRO::Core::ConstructTraitType::CTMax);
			if (ctt < minIntVal || ctt > maxIntVal) {
				InstRO::raise_exception(getId() + ": Cannot parse ConstructTraitType due to invalid integer value '" + std::to_string(ctt) + "'");
			} else {
				return static_cast<InstRO::Core::ConstructTraitType>(ctt);
			}
		} else if (cttValue.IsString()) {
			std::string cttString = cttValue.GetString();
			if (cttString == "min") {
				return InstRO::Core::ConstructTraitType::CTMin;
			} else if (cttString == "expression") {
				return InstRO::Core::ConstructTraitType::CTExpression;
			} else if (cttString == "statement") {
				return InstRO::Core::ConstructTraitType::CTStatement;
			} else if (cttString == "loopStatement") {
				return InstRO::Core::ConstructTraitType::CTLoopStatement;
			} else if (cttString == "conditionalStatement") {
				return InstRO::Core::ConstructTraitType::CTConditionalStatement;
			} else if (cttString == "scopeStatement") {
				return InstRO::Core::ConstructTraitType::CTScopeStatement;
			} else if (cttString == "simpleStatement") {
				return InstRO::Core::ConstructTraitType::CTSimpleStatement;
			} else if (cttString == "wrappableStatement") {
				return InstRO::Core::ConstructTraitType::CTWrappableStatement;
			} else if (cttString == "function") {
				return InstRO::Core::ConstructTraitType::CTFunction;
			} else if (cttString == "fileScope") {
				return InstRO::Core::ConstructTraitType::CTFileScope;
			} else if (cttString == "globalScope") {
				return InstRO::Core::ConstructTraitType::CTGlobalScope;
			} else if (cttString == "max") {
				return InstRO::Core::ConstructTraitType::CTMax;
			} else {
				InstRO::raise_exception(getId() + ": Unknown ConstructTraitType '" + cttString + "'");
			}
		} else {
			InstRO::raise_exception(getId() + ": Cannot parse ConstructTraitType due to unknown value type");
		}
	} else {
		InstRO::raise_exception(getId() + ": Cannot find member '" + std::string(memberName) + "'");
	}
}

void ConfigurationParsingContext::expectInputPasses(std::initializer_list<unsigned int> numberOfPasses) const {
	std::string numberBuffer;
	numberBuffer.reserve(numberOfPasses.size());

	bool foundFit = false;
	bool first = true; // whether this is the first element
	auto next = numberOfPasses.begin();
	while (next != numberOfPasses.end()) {
		auto current = next++;

		if (inputPasses.size() == *current) {
			foundFit = true;
			break;
		}

		// append checked number of passes to the buffer for the potential error message
		numberBuffer += std::to_string(*current);
		if (next != numberOfPasses.end()) {
			// append either 'or' or ','
			if (first) {
				numberBuffer += " or ";
				first = false;
			} else {
				numberBuffer += ", ";
			}
		}
	}

	if (!foundFit) {
		std::string passWord = (inputPasses.size() == 1) ? "pass" : "passes";
		InstRO::raise_exception(getId() + ": Expected " + numberBuffer + " input " + passWord + ", but got " + std::to_string(inputPasses.size()));
	}
}


// BaseConfigurationPassRegistry

ConfigurationPassRegistry::PassParser BaseConfigurationPassRegistry::lookup(const std::string &passType) {
	auto passRegistryIter = passRegistry.find(passType);
	if (passRegistryIter == passRegistry.end()) {
		return PassParser();
	}

	return passRegistryIter->second;
}

PassFactory* BaseConfigurationPassRegistry::getFactory() {
	return factory;
}

void BaseConfigurationPassRegistry::registerPass(const std::string &typeName, const PassParser &parser) {
	// print a warning if a parser has already been registered for the specified id
	if (passRegistry.find(typeName) != passRegistry.end()) {
		std::cerr << "Warning: a parser has already been registered for type '" << typeName << "'. Skipping..." << std::endl;
		return;
	}

	passRegistry.insert(std::make_pair(typeName, parser));
}
