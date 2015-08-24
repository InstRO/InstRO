#include "instro/utility/ConfigurationLoader.h"

#include "instro/utility/exception.h"

#include "rapidjson/filereadstream.h"

using namespace InstRO;
using namespace InstRO::Utility;

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

void ConfigurationParser::parseFile(const std::string &filename) {
	char buffer[1024];
	FILE *file = std::fopen(filename.c_str(), "r");
	if (!file) {
		std::cerr << "Failed to open file '" << filename << "'" << std::endl;
		return;
	}
	
	rapidjson::FileReadStream readStream (file, buffer, sizeof(buffer));
	doc.ParseStream(readStream);
	
	std::fclose(file);
	
	if (doc.HasParseError()) {
		std::cerr << "An error occurred while parsing the JSON document at position " << doc.GetErrorOffset() << std::endl;
	}
	
	
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
	
	auto passParser = loader.passRegistry->lookup(passType);
	Pass *pass = passParser(context);
	loader.passes.insert(std::make_pair(passId, pass));
	
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
	
	if (inputs != passValue.MemberEnd()) {
		rapidjson::Value &inputsValue = inputs->value;
		if (!inputsValue.IsArray()) {
			InstRO::raise_exception(std::string("The 'inputs' member of '") + passValue["id"].GetString() + "' must be an array");
		}
		
		inputPasses.reserve(inputsValue.Size());
		for (auto inputValueIter = inputsValue.Begin(); inputValueIter != inputsValue.End(); ++inputValueIter) {
			std::string inputId = inputValueIter->GetString();
			Pass *inputPass = loader.getPass(inputId);
			if (!inputPass) {
				// pass has not been parsed yet (id mentioned before declaration)
				rapidjson::Value *inputPassValue = findPassValue(inputId);
				if (inputPassValue) {
					inputPass = parsePass(*inputPassValue);
				} else {
					// failed to find the declaration of that id
					InstRO::raise_exception("No pass with id '" + inputId + "' has been declared");
				}
			}
			
			inputPasses.push_back(inputPass);
		}
	}

	return inputPasses;
}

std::vector<std::string> ConfigurationParsingContext::getStringArguments(const char* memberName) {
	std::vector<std::string> arguments;
	auto memberIter = passValue.FindMember(memberName);
	
	if (memberIter != passValue.MemberEnd()) {
		rapidjson::Value &argumentsValue = memberIter->value;
		if (!argumentsValue.IsArray()) {
			InstRO::raise_exception("The '" + std::string(memberName) + "' member of '" + passValue["id"].GetString() + "' must be an array");
		}
		
		arguments.reserve(argumentsValue.Size());
		for (auto argValueIter = argumentsValue.Begin(); argValueIter != argumentsValue.End(); ++argValueIter) {
			arguments.push_back(argValueIter->GetString());
		}
	}
	
	return arguments;
}

ConfigurationPassRegistry::PassParser ConfigurationPassRegistry::lookup(const std::string &passType) {
	auto passRegistryIter = passRegistry.find(passType);
	if (passRegistryIter == passRegistry.end()) {
		InstRO::raise_exception("Unknown pass type: " + passType);
	}
	
	return passRegistryIter->second;
}