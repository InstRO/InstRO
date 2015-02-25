#ifndef INSTRO_ROSE_ROSEPASS_H
#define INSTRO_ROSE_ROSEPASS_H

#include "instro/core/Pass.h"

namespace InstRO
{
	namespace Rose{
		class RosePass: public Pass
		{
		};
		class Adapter:public RosePass
		{
		};
		class Selector:public RosePass
		{
		};
		class Transformer: public RosePass
		{
		};
	}
};
#endif