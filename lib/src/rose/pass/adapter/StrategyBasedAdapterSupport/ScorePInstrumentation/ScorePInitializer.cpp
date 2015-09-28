#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ScorePInstrumentation/ScorePInitializer.h"

namespace InstRO {
	namespace Rose {
		namespace Adapter {
			namespace StrategyBasedAdapterSupport{
void ScorePInitializer::includeToolHeadersToFile(SgFile* file) {

	if (isIncludedIn(file)) return;

	SageInterface::insertHeader("scorep/SCOREP_User.h", PreprocessingInfo::before, true, isSgSourceFile(file)->get_globalScope());

	setIncludedIn(file);
}

}}}}