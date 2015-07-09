#ifndef INSTRO_TOOLING_GRAMMARINTERFACE_H
#define INSTRO_TOOLING_GRAMMARINTERFACE_H
#include <memory>	// We need shared pointers
#include <list>		 // We use List in the GrammarInterface
#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Tooling {
namespace GrammarInterface {
typedef enum {
	GTFor,
	GTOpenMP,
	GTDoWhile,
	GTExpression,
	GTReturn,
	GTOperator,
	GTFunction,
	GTMemberFunction
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
