#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ToolInitializer.h"

namespace InstRO {
	namespace Rose {
		namespace Adapter {
			namespace StrategyBasedAdapterSupport{

				ToolInitializer::~ToolInitializer() {

				}

				void ToolInitializer::includeToolHeadersToFile(SgFile* file) {
					setIncludedIn(file);
				}

				std::vector<SgDeclarationStatement*> ToolInitializer::constructToolDeclarationForMainFile(SgFile* main) {
					setDeclaredIn(main);
					return std::vector<SgDeclarationStatement*>();
				}

				std::vector<SgDeclarationStatement*> ToolInitializer::constructToolDeclarationForLocalFile(SgFile* file) {
					setDeclaredIn(file);
					return std::vector<SgDeclarationStatement*>();
				}

				std::vector<SgStatement*> ToolInitializer::constructToolInitializationIn(SgScopeStatement* scope) {
					setInitializedIn(scope);
					return std::vector<SgStatement*>();
				}

				std::vector<SgStatement*> ToolInitializer::constructToolShutdownIn(SgScopeStatement* scope) {
					setShutdownIn(scope);
					return std::vector<SgStatement*>();
				}

				void ToolInitializer::setIncludedIn(SgFile* file) {
					isIncluded.insert(file);
				}

				void ToolInitializer::setDeclaredIn(SgFile* file) {
					isDeclared.insert(file);
				}

				void ToolInitializer::setInitializedIn(SgScopeStatement* scope) {
					isInitialized.insert(scope);
				}

				void ToolInitializer::setShutdownIn(SgScopeStatement* scope) {
					isShutdown.insert(scope);
				}

				bool ToolInitializer::isIncludedIn(SgFile* file) {
					return isIncluded.count(file) > 0;
				}

				bool ToolInitializer::isDeclaredIn(SgFile* file) {
					return isDeclared.count(file) > 0;
				}

				bool ToolInitializer::isInitializedIn(SgScopeStatement* scope) {
					return isInitialized.count(scope) > 0;
				}

				bool ToolInitializer::isShutdownIn(SgScopeStatement* scope) {
					return isShutdown.count(scope) > 0;
				}


			}
		}
	}
}