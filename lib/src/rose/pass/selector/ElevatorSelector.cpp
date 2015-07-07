#include "instro/core/ConstructSet.h"
#include "instro/core/Singleton.h"
//#include "instro/rose/core/RosePassImplementation.h"
#include "instro/rose/pass/selector/ElevatorSelector.h"
#include "instro/tooling/AnalysisInterface.h"

namespace InstRO {
	namespace Rose {
		namespace Selector {

			void ConstructRaisingElevator::execute() {
			};
			void ConstructRaisingElevator::init(){
			};
			void ConstructRaisingElevator::execute(){
				auto constructElevator = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator();
				output=constructElevator->raise(input->getOutput(),this->minLevel);
			};
			void ConstructRaisingElevator::finalize(){
			};
			void ConstructRaisingElevator::releaseOutput(){
				output->clear();
			};
			Core::ConstructSet *ConstructRaisingElevator::getOutput(){
				return output.get();
			};

			void ConstructLoweringElevator::execute() {
			};
			void ConstructLoweringElevator::init(){
			};
			void ConstructLoweringElevator::execute(){
				auto constructElevator = InstRO::getInstrumentorInstance()->getAnalysisManager()->getCSElevator();
				output = constructElevator->lower(input->getOutput(), this->maxLevel);
			};
			void ConstructLoweringElevator::finalize(){
			};
			void ConstructLoweringElevator::releaseOutput(){
				output->clear();
			};
			Core::ConstructSet *ConstructLoweringElevator::getOutput(){
				return output.get();
			};



		}
	}
}