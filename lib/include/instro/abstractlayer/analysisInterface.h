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
			std::vector<ExtendedCallGraphNode*> findNodes(Core::ConstructSet * cs);
			//std::vector<ExtendedCallGraphNode*> findNodes(GrammarInterface::GrammerTypes type);
		};
	}

	class AnalysisManager
	{
	public:
		ExtendedCallGraph::ExtendedCallGraph * getECG();
		ControlFlowGraph::ControlFlowGraph * getCFG();
		ConstructElevator::ConstructElevator * getCSElevator();
		GrammarInterface::ConstructToGrammarTypeMapper *getConstructToGrammarTypeMaper();
		GrammarInterface::RequestCSByGrammarTypeInterface * getRequestCSByGrammarTypeInterface();
	};
	AnalysisManager * analysisManager;
}
}