#include "instro/tooling/analysisInterface.h"
#include "instro/example/ExampleConstructSet.h"

namespace InstRO
{
	namespace Example
	{
		namespace Tooling
		{



		
				class ExampleControlFlowGraph :public InstRO::Tooling::ControlFlowGraph::ControlFlowGraph
				{
				};

		

				/* CI: The ConstructSet class is intended to be specialized for each compiler interface. It provides the basic mechanisms to specify what construct level are contained. */
				class ExampleConstructElevator:public InstRO::Tooling::ConstructElevator::ConstructElevator
				{


					// This is the implicit way, that the PassManager will allways apply
					virtual InstRO::Core::ConstructSet & raise(InstRO::Core::ConstructSet & input, InstRO::Core::ConstructLevelType cl) override {
						return Example::Core::ExampleConstructSet();
					}
					// This is an explicit function used in very rare circumstances by e.g. a specialized selection pass (if at all)
					virtual InstRO::Core::ConstructSet & lower(InstRO::Core::ConstructSet & input, InstRO::Core::ConstructLevelType cl) {
						return Example::Core::ExampleConstructSet();
					}

					virtual InstRO::Core::ConstructSet & crop(InstRO::Core::ConstructSet & input, InstRO::Core::ConstructLevelType min, InstRO::Core::ConstructLevelType max)
					{
						return Example::Core::ExampleConstructSet();
					}
				};


				class ExampleGrammarInterface:public InstRO::Tooling::GrammarInterface::GrammarInterface
				{
					//class ConstructSetToGrammarTypeMapper
					virtual std::list<InstRO::Tooling::GrammarInterface::GrammarTypesType> getGrammerTypes(const InstRO::Core::ConstructSet & cs){
						return std::list<InstRO::Tooling::GrammarInterface::GrammarTypesType>();
					}

					//class RequestCSByGrammarTypeInterface
					virtual InstRO::Core::ConstructSet && getConstructsByType(const InstRO::Tooling::GrammarInterface::GrammarTypesType & types) override {
						return Example::Core::ExampleConstructSet();
					};

				};
	
				class ExampleExtendedCallGraph:public InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraph
				{
				public:
					std::vector<InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode*> findNodes(InstRO::Core::ConstructSet * cs) override {
						return std::vector<InstRO::Tooling::ExtendedCallGraph::ExtendedCallGraphNode*>();
					};
					//std::vector<ExtendedCallGraphNode*> findNodes(GrammarInterface::GrammerTypes type);
				};


			class ExampleAnalysisManager:public InstRO::Tooling::AnalysisManager
			{
			protected:
				ExampleExtendedCallGraph * ecg;
				ExampleConstructElevator * ce;
				ExampleControlFlowGraph * cfg;
			public:
				ExampleAnalysisManager() :ecg(NULL), ce(NULL), cfg(NULL){
				};

				ExampleExtendedCallGraph * getECG(){
					if (ecg == NULL)
						ecg = new ExampleExtendedCallGraph();
					return ecg; 
				};
				ExampleControlFlowGraph * getCFG(){ return new ExampleControlFlowGraph(); };
				ExampleConstructElevator * getCSElevator() {
					return new ExampleConstructElevator();
				}
				ExampleGrammarInterface *getGrammarInterface(){
					return new ExampleGrammarInterface();
				}
			};

		}
	}
}