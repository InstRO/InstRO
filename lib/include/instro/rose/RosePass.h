#ifndef INSTRO_ROSE_ROSEPASS_H
#define INSTRO_ROSE_ROSEPASS_H

#include "instro/core/Pass.h"

namespace InstRO
{
	namespace Rose{
		class RosePass: public Pass
		{
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
		};
		}
		namespace Transformer
		{
		class Transformer: public RosePass
		{
		};
		}
	}
};
#endif