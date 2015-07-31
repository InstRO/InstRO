#ifndef SCOREP_ADAPTER_BUILDER_H
#define SCOREP_ADAPTER_BUILDER_H


#include "instro/adapterbuilder/GenericAdapterBuilder.h"

#include "ScorePStatementWrapperStrategy.h"
#include "ScorePLoopIterationStrategy.h"
#include "ScorePFunctionScopeStrategy.h"

#include "ScorePInitializer.h"

namespace InstRO {

/**
 * \brief Composition for ScoreP instrumentation
 *
 * \ingroup Adapterbuilder
 *
 *\author Matthias Zoellner
 */
class ScorePAdapterBuilder : public GenericAdapterBuilder {

public:
	ScorePAdapterBuilder(Selector* decidingSelector);

	virtual ~ScorePAdapterBuilder();

private:
	ScorePInitializer initializer;

	ScorePStatementWrapperStrategy my_strategy;
	ScorePLoopIterationStrategy my_strategy2;
	ScorePFunctionScopeStrategy my_strategy3;
};

}

#endif

