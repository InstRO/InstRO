#include "instro/adapterbuilder/ScorePInstrumentation/ScorePAdapterBuilder.h"

using namespace InstRO;


ScorePAdapterBuilder::ScorePAdapterBuilder(Selector* decidingSelector) :
		GenericAdapterBuilder(decidingSelector), initializer(),
		my_strategy(&initializer), my_strategy2(&initializer), my_strategy3(&initializer) {

	// register ScoreP instrumentation strategy
	this->addInstrumentationStrategy(&my_strategy2);
	this->addInstrumentationStrategy(&my_strategy3);
	this->addInstrumentationStrategy(&my_strategy);
}


ScorePAdapterBuilder::~ScorePAdapterBuilder() {

}

