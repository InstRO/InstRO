#ifndef INSTRO_UTILITY_CONFIGURATIONLOADER
#define INSTRO_UTILITY_CONFIGURATIONLOADER

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>
#include <initializer_list>

#include "instro/core/ConstructSet.h"
#include "instro/core/PassFactory.h"

#include "rapidjson/document.h"

namespace InstRO {

// Forward declarations for the pass factories
namespace Rose {
	class RosePassFactory;
}

namespace Clang {
	class PassFactory;
}


namespace Utility {

	/// \brief Represents the context of a pass that is being parsed and provides methods to retrieve relevant information.
	///
	/// If a pass is being parsed, all its input passes are guaranteed to have been processed beforehand and are available in \c inputPasses.
	struct ConfigurationParsingContext {
		ConfigurationParsingContext(rapidjson::Value &passValue, const std::vector<Pass*> &inputPasses) : passValue(passValue), inputPasses(inputPasses) {}

		/// \brief Gets the identifier of the pass that is parsed.
		std::string getId() const;
		/// \brief Gets the type of the pass that is parsed.
		std::string getType() const;

		std::string getStringArgument(const char* memberName) const;
		std::vector<std::string> getStringArguments(const char* memberName = "args") const;

		/// \brief Gets the InstRO::Core::ConstructTraitType of the specified member.
		///
		/// The value may be either an integer or a textual name (e.g. 'function', 'fileScope').
		InstRO::Core::ConstructTraitType getConstructTraitType(const char* memberName) const;

		/// \brief Checks whether the specified number of input passes are available and raises an exception if not.
		void expectInputPasses(std::initializer_list<unsigned int> numberOfPasses) const;

		rapidjson::Value &passValue;
		std::vector<Pass*> inputPasses;
	};

	/// \brief Interface for querying for pass parsers which handle the creation of individual passes.
	class ConfigurationPassRegistry {
		public:
			typedef std::function<Pass*(ConfigurationParsingContext&)> PassParser;

			virtual ~ConfigurationPassRegistry() {}

			virtual PassParser lookup(const std::string &passType)=0;
	};

	/// \brief An abstract ConfigurationPassRegistry that stores its PassParser instances in a map.
	class BaseConfigurationPassRegistry : public ConfigurationPassRegistry {
		public:
			BaseConfigurationPassRegistry(PassFactory *factory) : factory(factory) {}
			virtual ~BaseConfigurationPassRegistry() {}

			PassParser lookup(const std::string &passType) override;

		protected:
			PassFactory* getFactory();
			/// \brief Registers a new PassParser instance.
			///
			/// Prints a warning on standard error if a parser has already been registered for that type ands skips the registration.
			void registerPass(const std::string &typeName, const PassParser &parser);

		private:
			PassFactory * const factory;
			std::unordered_map<std::string, PassParser> passRegistry;

	};

	/// \brief Offers the ability to load pass configurations from a JSON file.
	///
	/// The passes inside the configuration file are resolved using a ConfigurationPassRegistry.
	/// The JSON file consists of an array of objects where each object describes a InstRO::Pass.
	/// The following fields have a common semantic across passes:
	/// \li \c type (mandatory) specifies the name of the pass that should be created.
	/// \li \c id (mandatory) specifies a unique identifier for the pass instance so that other passes may refer to the instance.
	/// \li \c inputs (optional) specifies an array of pass ids that serve as input for the pass.
	///
	/// Please note that the order of passes inside the JSON document is redundant, as (acyclic) dependencies are automatically resolved.
	/// \author Simon Reuß
	class ConfigurationLoader {
		friend class ConfigurationParser;

		public:
			typedef std::unordered_map<std::string, Pass*> PassMap;

			ConfigurationLoader(std::unique_ptr<ConfigurationPassRegistry> passRegistry);
			virtual ~ConfigurationLoader() {}

			void load(const std::string &filename);

			PassMap getPasses() const;
			Pass* getPass(const std::string &passId) const;

		protected:
			std::unique_ptr<ConfigurationPassRegistry> passRegistry;

			PassMap passes;
	};

	/// \brief Used by ConfigurationLoader to handle the actual parsing of a JSON file.
	class ConfigurationParser {
	public:
		ConfigurationParser(ConfigurationLoader &loader) : loader(loader) {}

		void parseFile(const std::string &filename);

	private:
		ConfigurationLoader& loader;
		rapidjson::Document doc;

		rapidjson::Value* findPassValue(const std::string &passId);

		Pass* parsePass(rapidjson::Value &passValue);
		std::vector<Pass*> getInputPasses(rapidjson::Value &passValue);
	};

	/// \brief A ConfigurationPassRegistry providing all available Rose passes.
	/// \todo Move declaration to a separate header if a direct dependency to Rose specific code is used in the declaration.
	class RoseConfigurationPassRegistry : public BaseConfigurationPassRegistry {
	public:
		RoseConfigurationPassRegistry(InstRO::Rose::RosePassFactory *factory);
	};

	/// \brief A ConfigurationPassRegistry providing all available Clang passes.
	/// \todo Move declaration to a separate header if a direct dependency to Clang specific code is used in the declaration.
	class ClangConfigurationPassRegistry : public BaseConfigurationPassRegistry {
	public:
		ClangConfigurationPassRegistry(InstRO::Clang::PassFactory *factory);
	};

}
}

#endif
