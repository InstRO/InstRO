#ifndef INSTRO_CORE_PASSIMPLEMENTATION_H
#define INSTRO_CORE_PASSIMPLEMENTATION_H

#include <unordered_map>

#include "instro/core/ConstructSet.h"

namespace InstRO {
	class Pass;
	namespace Core {
		namespace Support {
			/*
			 * Support class to hand the inputs from the pass manager to the
			 * PassImplementation
			 */
			class InputAggregation {
			public:
				InputAggregation(){};
				InputAggregation(std::unordered_map<InstRO::Pass *, ConstructSet *> m) : pToCMap(m){}
				ConstructSet *getConstructSet(InstRO::Pass *p){
					return pToCMap[p];
				}

			private:
				// CI: Us the Pass as a AccessHandle to the RespectiveConstruct set
				std::unordered_map<InstRO::Pass *, InstRO::Core::ConstructSet *> pToCMap;
			};
		}
	


	class ChannelConfiguration{
	protected:
		std::vector<Pass*> inputChannelPasses;
	public:
		// CI: Empty Configuration - No Input Passes used
		ChannelConfiguration(){
		};

		ChannelConfiguration(Pass * p1){
			inputChannelPasses.push_back(p1);
		}

		template <class ... PassList> ChannelConfiguration(Pass * p1, PassList... passes){
			inputChannelPasses.insert(inputChannelPasses.begin(), { p1, passes... });
		}

		void setConstructLevel(Pass* inputPass, int minLevel, int maxLevel){
		}
	};
	}
	/*
	 * This class is the user-interface to create his own pass.
	 * One needs to inherit from this class and implement a compiler dependent pass.
	 * Using the getInput(passId) one can retrieve the ConstructSet of one of the
	 * predecessors.
	 */

	namespace Core{
		class PassImplementation {
		private:
			Core::ChannelConfiguration cfg;
		protected:

			Core::ChannelConfiguration & getChannelCFG(){
				return cfg;
			}
		public:
			Core::ChannelConfiguration const channelCFG(){
				return cfg;
			}

			PassImplementation() = delete;

			PassImplementation(Core::ChannelConfiguration cfg) :cfg(cfg){
			}

			virtual void init() = 0;
			virtual void execute() = 0;
			virtual void finalize() = 0;
			virtual void releaseOutput() = 0;
			virtual Core::ConstructSet *getOutput() = 0;

			void setInputAggregation(InstRO::Core::Support::InputAggregation ia) {
				this->ia = ia;
			}

		protected:
			InstRO::Core::ConstructSet * getInput(InstRO::Pass *pId){
				return ia.getConstructSet(pId);
			}

		private:
			InstRO::Core::Support::InputAggregation ia;
		};


	}
}
#endif
