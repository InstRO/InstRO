#include "instro/core/ConstructSet.h"

namespace InstRO
{
namespace AnalysisLayer
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

		};
	}
	namespace GrammarInterface
	{
		typedef enum {
			For,
			OpenMP,
			DoWhile,
			Expression,
			Return
		} GrammerTypes;

		class ConstructToGrammarTypeMapper
		{
		};
		class RequestCSByGrammarTypeInterface
		{

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
		ConstructElevator::ConstructElevator * getCSElevator();
		GrammarInterface::ConstructToGrammarTypeMapper *getConstructToGrammarTypeMaper();
		GrammarInterface::RequestCSByGrammarTypeInterface * getRequestCSByGrammarTypeInterface();
	};
	AnalysisManager * analysisManager;
}
}