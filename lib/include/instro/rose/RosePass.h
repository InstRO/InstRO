#ifndef INSTRO_ROSE_ROSEPASS_H
#define INSTRO_ROSE_ROSEPASS_H

#include "instro/core/Pass.h"

namespace InstRO
{
	namespace Rose{
		class RosePass: public Pass
		{
		public:
			virtual std::string className(){return std::string("InstRO::Rose::Selector::RosePass");};
		};
		namespace Adapters{
		class Adapter:public RosePass
		{
			virtual std::string className(){return std::string("InstRO::Rose::Selector::Adapter");};
		};
		}
		namespace Selectors
		{

		class Selector:public RosePass
		{
		public:
			virtual std::string className(){return std::string("InstRO::Rose::Selector::Selector");};
		};
		}
		namespace Transformer
		{
		class Transformer: public RosePass
		{
			public:
			virtual std::string className(){return std::string("InstRO::Rose::Selector::Transformer");};
		};
		}
	}
};
#endif