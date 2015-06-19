#ifndef INSTRO_ROSE_ROSEPASS_H
#define INSTRO_ROSE_ROSEPASS_H

#include "instro/core/Pass.h"

namespace InstRO
{
namespace Test{
	class TestPass : public PassImplementation
		{
		};
namespace Adapters{
	class Adapter :public TestPass
	{
	};
}

namespace Selectors
		{
// Simple test implementation of a CPS

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