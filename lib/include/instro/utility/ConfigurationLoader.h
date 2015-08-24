#ifndef INSTRO_UTILITY_CONFIGURATIONLOADER
#define INSTRO_UTILITY_CONFIGURATIONLOADER

#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

#include "instro/core/PassFactory.h"

#include "rapidjson/document.h"

namespace InstRO {
	
namespace Rose {
	class RosePassFactory;
}
	
namespace Utility {
	
	struct ConfigurationParsingContext {
		ConfigurationParsingContext(rapidjson::Value &passValue, const std::vector<Pass*> &inputPasses) : passValue(passValue), inputPasses(inputPasses) {}
		
		std::vector<std::string> getStringArguments(const char* memberName = "args");
		
		rapidjson::Value &passValue;
		std::vector<Pass*> inputPasses;
	};
	
	class ConfigurationPassRegistry {
		public:
			typedef std::function<Pass*(ConfigurationParsingContext&)> PassParser;
			
			ConfigurationPassRegistry(PassFactory *factory) : factory(factory) {}
			virtual ~ConfigurationPassRegistry() {}
			
			PassParser lookup(const std::string &passType);
		protected:
			
			PassFactory * const factory;
			std::unordered_map<std::string, PassParser> passRegistry;
	};
	
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
	
	class RoseConfigurationPassRegistry : public ConfigurationPassRegistry {
	public:
		RoseConfigurationPassRegistry(InstRO::Rose::RosePassFactory *factory);
	};
	
//	class ClangConfigurationPassRegistry : public ConfigurationPassRegistry {
//		
//	};
	
}
}

#endif