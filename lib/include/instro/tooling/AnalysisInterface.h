#ifndef INSTRO_ANALYSIS_INTERFACE
#define INSTRO_ANALYSIS_INTERFACE

#include <memory> // We need shared pointers
#include <list> // We use List in the GrammarInterface
#include "instro/core/ConstructSet.h"

namespace InstRO
{
namespace Tooling
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
		/* CI: The ConstructSet class is intended to be specialized for each compiler interface. It provides the basic mechanisms to specify what construct level are contained. */
		class ConstructElevator
		{
		public:

			// This is the implicit way, that the PassManager will allways apply
			virtual Core::ConstructSet raise(Core::ConstructSet & input, Core::ConstructLevelType cl) = 0;
			// This is an explicit function used in very rare circumstances by e.g. a specialized selection pass (if at all)
			virtual Core::ConstructSet lower(Core::ConstructSet & input, Core::ConstructLevelType cl) = 0;
			// Crop Construct up to, or down to a level
			virtual Core::ConstructSet crop(Core::ConstructSet & input, Core::ConstructLevelType min, Core::ConstructLevelType max) = 0;
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
			virtual std::list<GrammarTypesType> getGrammerTypes(const Core::ConstructSet & cs) = 0;
		
		//class RequestCSByGrammarTypeInterface
			virtual Core::ConstructSet getConstructsByType(const GrammarTypesType & types) = 0;

		};
	}
	namespace ExtendedCallGraph{
		class ExtendedCallGraphNode
		{
			virtual Core::ConstructSet * getCS() = 0;

		};
		class ExtendedCallGraph
		{
		public:
			virtual std::vector<ExtendedCallGraphNode*> findNodes(Core::ConstructSet * cs) = 0;
			//std::vector<ExtendedCallGraphNode*> findNodes(GrammarInterface::GrammerTypes type);
		};
	}

	class AnalysisManager
	{
	public:
		virtual ExtendedCallGraph::ExtendedCallGraph * getECG() = 0;
		virtual ControlFlowGraph::ControlFlowGraph * getCFG() = 0;
		virtual ConstructElevator::ConstructElevator * getCSElevator() = 0;
		virtual GrammarInterface::GrammarInterface *getGrammarInterface() = 0;
	};

	extern std::shared_ptr<AnalysisManager> analysisManager;

}
}
#endif
