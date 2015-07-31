#ifndef SCORE_P_INITIALIZER_H__
#define SCORE_P_INITIALIZER_H__

#include "instro/adapterbuilder/ToolInitializer.h"

namespace InstRO {

/**
 * \brief ScoreP tool initialization
 *
 * \author Matthias Zoellner
 */
class ScorePInitializer : public ToolInitializer {
public:

	/** Include the tool headers to the provided file */
	void includeToolHeadersToFile(SgFile* file);

}; // class ScorePInitializer

} // namespace InstRO

#endif // SCORE_P_INITIALIZER_H__

