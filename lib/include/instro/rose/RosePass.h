#ifndef INSTRO_ROSE_ROSEPASS_H
#define INSTRO_ROSE_ROSEPASS_H

#include "instro/core/PassImplementation.h"

namespace InstRO
{
	namespace Rose{
		class RosePass: public PassImplementation
		{
		public:
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