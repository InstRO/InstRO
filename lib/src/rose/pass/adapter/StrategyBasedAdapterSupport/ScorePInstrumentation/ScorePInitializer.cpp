#include "instro/adapterbuilder/ScorePInstrumentation/ScorePInitializer.h"

using namespace InstRO;

void ScorePInitializer::includeToolHeadersToFile(SgFile* file) {

	if (isIncludedIn(file)) return;

	SageInterface::insertHeader("scorep/SCOREP_User.h", PreprocessingInfo::before, true, isSgSourceFile(file)->get_globalScope());

	setIncludedIn(file);
}

