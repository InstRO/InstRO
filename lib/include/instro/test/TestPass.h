#ifndef INSTRO_ROSE_ROSEPASS_H
#define INSTRO_ROSE_ROSEPASS_H

#include "instro/core/Pass.h"

namespace InstRO
{
	namespace Test{
		class TestPass: public Pass
		{
		};
		namespace Adapters{
		class Adapter:public TestPass
		{
		};
		}
		namespace Selectors
		{

		class Selector:public TestPass
		{
		};
		}
		namespace Transformer
		{
		class Transformer: public TestPass
		{
		};
		}
	}
};
#endif