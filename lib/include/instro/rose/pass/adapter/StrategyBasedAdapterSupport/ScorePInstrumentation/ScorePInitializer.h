#ifndef INSTRO_ROSE_PASS_ADAPTER_STRATEGYBASEDADAPTER_SCOREPINSTRUMENTATION_SCORE_P_INITIALIZER_H__
#define INSTRO_ROSE_PASS_ADAPTER_STRATEGYBASEDADAPTER_SCOREPINSTRUMENTATION_SCORE_P_INITIALIZER_H__

#include "instro/rose/pass/adapter/StrategyBasedAdapterSupport/ToolInitializer.h"

namespace InstRO {
	namespace Rose {
		namespace Adapter {
			namespace StrategyBasedAdapterSupport{

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

}}}} // namespace InstRO

#endif // SCORE_P_INITIALIZER_H__

