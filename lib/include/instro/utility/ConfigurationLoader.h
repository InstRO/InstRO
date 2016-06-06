#ifndef INSTRO_UTILITY_CONFIGURATIONLOADER_H
#define INSTRO_UTILITY_CONFIGURATIONLOADER_H

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <initializer_list>

#include "instro/core/ConstructTraitType.h"
#include "instro/PassFactory.h"

#include "rapidjson/document.h"

namespace InstRO {

// Forward declarations for the pass factories
namespace Rose {
class RosePassFactory;
}

namespace Clang {
class ClangPassFactory;
}

namespace Utility {

/// \brief Represents the context of a pass that is being parsed and provides methods to retrieve relevant information.
///
/// If a pass is being parsed, all its input passes are guaranteed to have been processed beforehand and are available
/// in #inputPasses.
struct ConfigurationParsingContext {
	ConfigurationParsingContext(rapidjson::Value& passValue, const std::vector<Pass*>& inputPasses)
			: passValue(passValue), inputPasses(inputPasses) {}

	/// \brief Gets the identifier of the pass that is parsed.
	std::string getId() const;
	/// \brief Gets the type of the pass that is parsed.
	std::string getType() const;

	/// \brief Gets the string value of the specified member.
	///
	/// Raises an exception if the member does not exist.
	std::string getStringArgument(const char* memberName) const;
	/// \brief Gets the string value of the specified member or the given default value if the member does not exist.
	std::string getStringArgumentOrDefault(const char* memberName, const std::string& defaultArg) const;
	/// \brief Gets the array of string values of the specified member.
	///
	/// Raises an exception if the member does not exist or is not an array.
	std::vector<std::string> getStringArguments(const char* memberName = "args") const;

	/// \brief Gets the integer value of the specified member.
	///
	/// Raises an exception if the member does not exist.
	int getIntegerArgument(const char* memberName) const;

	/// \brief Gets the InstRO::Core::ConstructTraitType of the specified member.
	///
	/// The value may be either an integer or a textual name (e.g. 'function', 'fileScope').
	InstRO::Core::ConstructTraitType getConstructTraitType(const char* memberName) const;

	/// \brief Checks whether the specified number of input passes are available and raises an exception if not.
	///
	/// An exception is raised if the actual number of input passes does not match the expectation.
	///
	/// For example, if a pass expects either one or two input passes, \c expectInputPasses({1,2}) should be invoked.
	void expectInputPasses(std::initializer_list<unsigned int> numberOfPasses) const;

	/// \brief The JSON object representing the Pass.
	///
	/// Instead of accessing this directly, consider using one of the methods available to extract information.
	rapidjson::Value& passValue;
	/// \brief The Pass instances which serve as inputs for the Pass that is parsed.
	std::vector<Pass*> inputPasses;
};

/// \brief Interface for querying for pass parsers which handle the creation of individual passes.
class ConfigurationPassRegistry {
 public:
	/// A function that is able to parse passes of a specific type using the supplied ConfigurationParsingContext.
	typedef std::function<Pass*(ConfigurationParsingContext&)> PassParser;

	virtual ~ConfigurationPassRegistry() {}

	/// \brief Looks for a PassParser capable of parsing the specified pass type.
	///
	/// \return A valid PassParser or an empty function if no fitting PassParser has been registered.
	virtual PassParser lookup(const std::string& passType) = 0;
};

/// \brief A ConfigurationPassRegistry that stores its PassParser instances in a map
/// and uses a PassFactory to create the Pass instances.
/// This class provides parsers for the passes that do not depend on a specific compiler implementation (those creatable
/// by InstRO::PassFactory).
///
/// Subclasses should register all the PassParser they provide by calling #registerPass.
class BaseConfigurationPassRegistry : public ConfigurationPassRegistry {
 public:
	BaseConfigurationPassRegistry(PassFactory* factory);

	virtual ~BaseConfigurationPassRegistry() {}

	PassParser lookup(const std::string& passType) override;

 protected:
	PassFactory* getFactory();

	/// \brief Registers a new PassParser instance for the specified pass type.
	///
	/// Prints a warning on standard error if a parser has already been registered for that type ands skips the
	/// registration.
	void registerPass(const std::string& typeName, const PassParser& parser);

 private:
	PassFactory* const factory;
	std::unordered_map<std::string, PassParser> passRegistry;
};

/// \brief Offers the ability to load pass configurations from a JSON file.
///
/// The passes inside the configuration file are resolved using a ConfigurationPassRegistry.
/// The JSON file consists of an array of objects where each object describes a InstRO::Pass.
/// The following fields have a common semantic across passes:
/// - \c type (mandatory) specifies the name of the pass that should be created.
/// - \c id (mandatory) specifies a unique identifier for the pass instance so that other passes may refer to the
/// instance.
/// - \c inputs (optional) specifies an array of pass ids that serve as input for the pass.
///
/// Please note that the order of passes inside the JSON document is redundant because (acyclic) dependencies are
/// automatically resolved.
/// \author Simon Reuß
class ConfigurationLoader {
	friend class ConfigurationParser;

 public:
	typedef std::unordered_map<std::string, Pass*> PassMap;

	/// \brief Creates a new ConfigurationLoader instance that uses the specified ConfigurationPassRegistry to determine
	/// what passes are available and how they are parsed.
	///
	/// The ownership of the registry is transfered to the ConfigurationLoader.
	ConfigurationLoader(std::unique_ptr<ConfigurationPassRegistry> passRegistry);
	virtual ~ConfigurationLoader() {}

	/// \brief Parses the specified configuration file by creating the described passes.
	///
	/// The state of previous method invocations are not discarded. Therefore it is possible to load multiple
	/// configuration files so long as there are no cross dependencies between passes.
	void load(const std::string& filename);

	/// Retrieves a mapping of pass identifiers to pass instances.
	PassMap getPasses() const;
	/// Retrieves the pass with the specified identifier or \c nullptr if no pass is available for the identifier.
	Pass* getPass(const std::string& passId) const;

 protected:
	std::unique_ptr<ConfigurationPassRegistry> passRegistry;

	PassMap passes;
};

/// \brief Used by the ConfigurationLoader to handle the actual parsing of a specific JSON file.
///
/// The associated ConfigurationLoader supplies the ConfigurationPassRegistry for parsing of individual passes and the
/// storage for new passes.
class ConfigurationParser {
 public:
	ConfigurationParser(ConfigurationLoader& loader) : loader(loader) {}

	void parseFile(const std::string& filename);

 private:
	ConfigurationLoader& loader;
	rapidjson::Document doc;

	/// Retrieves the value of the pass with the specified identifier or \c nullptr if no pass can be found.
	rapidjson::Value* findPassValue(const std::string& passId);

	/// Parses the specified value into a Pass a registers it with the ConfigurationLoader.
	Pass* parsePass(rapidjson::Value& passValue);
	/// Parses potential dependencies of the pass represented by the specified value.
	std::vector<Pass*> getInputPasses(rapidjson::Value& passValue);
};

/// \brief A ConfigurationPassRegistry providing all available ROSE passes.
/// \todo Move declaration to a separate header if a direct dependency to ROSE specific code is used in the declaration.
class RoseConfigurationPassRegistry : public BaseConfigurationPassRegistry {
 public:
	RoseConfigurationPassRegistry(InstRO::Rose::RosePassFactory* factory);
};

/// \brief A ConfigurationPassRegistry providing all available Clang passes.
/// \todo Move declaration to a separate header if a direct dependency to Clang specific code is used in the
/// declaration.
class ClangConfigurationPassRegistry : public BaseConfigurationPassRegistry {
 public:
	ClangConfigurationPassRegistry(InstRO::Clang::ClangPassFactory* factory);
};
}
}

#endif
