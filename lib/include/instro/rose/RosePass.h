#ifndef INSTRO_ROSE_ROSEPASS_H
#define INSTRO_ROSE_ROSEPASS_H

#include "instro/core/PassImplementation.h"

namespace InstRO
{
	namespace Rose{
		class RosePass: public InstRO::Core::PassImplementation
		{
		public:
			RosePass(InstRO::Core::ChannelConfiguration ccfg) :PassImplementation(ccfg){}
		};

		class RoseLegcyPass:public RosePass{
		};

		namespace Adapters{
		class Adapter:public RosePass
		{
		};
		}
		namespace Selectors
		{

		class Selector:public RosePass
		{
		public:
		};
		}
		namespace Transformer
		{
		class Transformer: public RosePass
		{
			public:
		};
		}
	}
};
#endif