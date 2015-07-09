#ifndef INSTRO_TOOLING_GRAMMARINTERFACE_H
#define INSTRO_TOOLING_GRAMMARINTERFACE_H
#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Tooling {
namespace GrammarInterface {
typedef enum {
	GT_MIN_TYPE_NUMBER = 0,
	GTFor = 0,
	GTDoWhile,
	GTExpression,
	GTReturn,
	GTOperator,
	GTFunction,
	GTMemberFunction,
	// OpenMP Now
	GTOMPParallel,
	GTOMPParallelFor,
	GTOMPTask,
	GTOMPSection,
	GT_MAX_TYPE_NUMBER,
	GTInvalid
} GrammarTypesType;

class GrammarInterface {
 public:
	// class ConstructSetToGrammarTypeMapper
	virtual std::list<GrammarTypesType> getGrammerTypes(const Core::ConstructSet &cs) = 0;

	// class RequestCSByGrammarTypeInterface
	virtual std::unique_ptr<InstRO::Core::ConstructSet> getConstructsByType(const GrammarTypesType &types) = 0;
};

}	// GrammarInterface
}	// Tooling
}	// InstRO
#endif
