#include <set>
#include <vector>
#include "instro/core/ConstructSet.h"

namespace InstRO
{
	namespace Example
	{
		namespace Core
		{
			class ExampleConstruct: public InstRO::Core::Construct
			{};
#ifdef DEPRECATEDCS
class ExampleConstructSet :public InstRO::Core::ConstructSet
{
	protected:
		std::set<ExampleConstruct*> constructs;
		ExampleConstructSet(ExampleConstruct * construct){
	//		constructs.insert(construct);
		};
	public:
		ExampleConstructSet(){};
		virtual ::std::vector<InstRO::Core::ConstructLevelType> getConstructLevels() override{
					// InstRO::Core::ConstructTypesType type;
					return std::vector<InstRO::Core::ConstructLevelType>();
				}
				virtual InstRO::Core::ConstructLevelType getMaxConstructLevel() override {
					return InstRO::Core::ContstructLevelEnum::CLLoop;
				}
				virtual InstRO::Core::ConstructLevelType getMinConstructLevel() override {
					return InstRO::Core::ContstructLevelEnum::CLMin;
				}
				virtual void clear() override{
					constructs.clear();
				}
				virtual size_t size() override {
					return constructs.size();
				}
				/*

				virtual InstRO::Core::ConstructSet & intersect(InstRO::Core::ConstructSet & b) override {
					return InstRO::Example::Core::ExampleConstructSet();
				}*/

				/*
				virtual void add(ConstructSet * setB){
					ExampleConstructSet retSet;
					retSet.add(*this);
					retSet.add

				}*/
			//	virtual void add(ConstructSet & set) = NULL;
				//virtual ConstructSet intersect(ConstructSet b) = NULL;

				virtual void put(ConstructSet & input) {
					ExampleConstructSet in=dynamic_cast<ExampleConstructSet&>(input);
				//	put(in);
				}
				virtual void put(ExampleConstructSet & input) {
				//	for (auto iter = input.constructs.begin(); iter != input.constructs.end(); iter++);
						//constructs.insert(*iter);
				}
				virtual void erase(ConstructSet & s)
				{}

				// https://en.wikipedia.org/wiki/Set_(mathematics)
				virtual ConstructSet & combine(ConstructSet &){ return *( new ExampleConstructSet()); }
				virtual ConstructSet & intersect(ConstructSet &) { return ExampleConstructSet(); }
				virtual ConstructSet & relativecomplement(ConstructSet &) { return ExampleConstructSet(); }
				virtual ConstructSet & symmerticDifference(ConstructSet&) { return ExampleConstructSet(); }
			};
#endif
		}
	}
}