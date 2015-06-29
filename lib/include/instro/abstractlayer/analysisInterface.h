#include "instro/core/ConstructSet.h"

namespace InstRO
{
namespace AbstractLayer
{
	
	namespace ControlFlowGraph
	{
		class ControlFlowGraphNode
		{

		};
		class ControlFlowGraph
		{
		};

	}
	namespace ConstructElevator
	{
		class ConstructElevator
		{
			Core::ConstructSet raise(Core::ConstructSet input,Core::ConstructLevelType cl)
			{
			}
			Core::ConstructSet lower(Core::ConstructSet input,Core::ConstructLevelType cl)
			{
			}
		};
	}
	namespace GrammarInterface
	{
		typedef enum {
			For,
			OpenMP,
			DoWhile,
			Expression,
			Return,
			Operator,
		} GrammarTypesType;

		class GrammarInterface
		{
		//class ConstructSetToGrammarTypeMapper
			std::list<GrammarTypesType> getGrammerTypes(Core::ConstructSet cs){
				return std::list<GrammarTypesType>();
			}
		
		//class RequestCSByGrammarTypeInterface
			Core::ConstructSet getConstructsByType(GrammarTypesType types){
				return Core::ConstructSet();
			}

		};
	}

	namespace ExtendedCallGraph{
		class ExtendedCallGraphNode
		{
			Core::ConstructSet * getCS(){ return new Core::ConstructSet(); };

		};
		class ExtendedCallGraph
		{
		public:
			std::vector<ExtendedCallGraphNode*> findNodes(Core::ConstructSet * cs){ return std::vector<ExtendedCallGraphNode*>(); };
			//std::vector<ExtendedCallGraphNode*> findNodes(GrammarInterface::GrammerTypes type);
		};
	}

	class AnalysisManager
	{
	public:
		ExtendedCallGraph::ExtendedCallGraph * getECG(){ return new ExtendedCallGraph::ExtendedCallGraph(); };
		ControlFlowGraph::ControlFlowGraph * getCFG(){ return new ControlFlowGraph::ControlFlowGraph(); };
		ConstructElevator::ConstructElevator * getCSElevator() {
			return new ConstructElevator::ConstructElevator();
		};
		GrammarInterface::GrammarInterface *getGrammarInterface(){ 
			return new GrammarInterface::GrammarInterface();
		}
	};
}
}