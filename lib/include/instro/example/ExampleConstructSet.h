#ifndef EXAMPLE_CORE_CONSTRUCTSET
#define EXAMPLE_CORE_CONSTRUCTSET 0.1
#include <set>
#include <memory>
#include <vector>
#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Example {
namespace Core {
class ExampleConstruct : public InstRO::Core::Construct {
 public:
	 ExampleConstruct(InstRO::Core::ConstructTrait level, std::string nname) :Construct(level), name(nname), hierarchyPartent(nullptr){
		 constructTraits = InstRO::Core::ConstructTrait(level);
	};
	ExampleConstruct(InstRO::Core::ConstructTrait level, std::string nname, ExampleConstruct * parent) :Construct(level), name(nname), hierarchyPartent(parent){
		constructTraits = InstRO::Core::ConstructTrait(level);
	};
	std::string toString() { return name; };

 protected:
	std::string name;
	ExampleConstruct * hierarchyPartent;
};
namespace InfracstructureInterface {
	/*

	 class ExampleConstructSet :public InstRO::Core::ConstructSet {
	 public:
		 ExampleConstructSet(InstRO::Core::ConstructSet & cs){
			 constructs = cs.constructs;

		 };
		 ExampleConstructSet(){};
		 virtual void put(const std::shared_ptr<InstRO::Core::Construct>& construct){
			 InstRO::Core::ConstructSet::put(construct);
		 }
		 virtual void erase(const std::shared_ptr<InstRO::Core::Construct>& construct){
			 InstRO::Core::ConstructSet::erase(construct);
		 };
 //		virtual void put(ConstructSet cs);
 //		virtual void erase(ConstructSet cs);
 //		bool contains(const std::shared_ptr<InstRO::Core::Construct>& construct) const;
		 std::set<std::shared_ptr<InstRO::Core::Construct> >::iterator begin()
		 {
			 return InstRO::Core::ConstructSet::begin();
		 }
		 std::set<std::shared_ptr<InstRO::Core::Construct> >::iterator end(){
			 return InstRO::Core::ConstructSet::end();
		 }
		 std::set<std::shared_ptr<InstRO::Core::Construct> >::const_iterator  cbegin()const;
		 std::set<std::shared_ptr<InstRO::Core::Construct> >::const_iterator  cend()const;
	 };*/
}



#ifdef DEPRECATEDCS
class ExampleConstructSet : public InstRO::Core::ConstructSet {
 protected:
	std::set<ExampleConstruct *> constructs;
	ExampleConstructSet(ExampleConstruct *construct){
			//		constructs.insert(construct);
	};

 public:
	ExampleConstructSet(){};
	virtual ::std::vector<InstRO::Core::ConstructTraitType> getConstructLevels() override {
		// InstRO::Core::ConstructTypesType type;
		return std::vector<InstRO::Core::ConstructTraitType>();
	}
	virtual InstRO::Core::ConstructTraitType getMaxConstructLevel() override {
		return InstRO::Core::ContstructLevelEnum::CLLoop;
	}
	virtual InstRO::Core::ConstructTraitType getMinConstructLevel() override {
		return InstRO::Core::ContstructLevelEnum::CLMin;
	}
	virtual void clear() override { constructs.clear(); }
	virtual size_t size() override { return constructs.size(); }
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
	// virtual ConstructSet intersect(ConstructSet b) = NULL;

	virtual void put(ConstructSet &input) {
		ExampleConstructSet in = dynamic_cast<ExampleConstructSet &>(input);
		//	put(in);
	}
	virtual void put(ExampleConstructSet &input) {
		//	for (auto iter = input.constructs.begin(); iter != input.constructs.end(); iter++);
		// constructs.insert(*iter);
	}
	virtual void erase(ConstructSet &s) {}

	// https://en.wikipedia.org/wiki/Set_(mathematics)
	virtual ConstructSet &combine(ConstructSet &) { return *(new ExampleConstructSet()); }
	virtual ConstructSet &intersect(ConstructSet &) { return ExampleConstructSet(); }
	virtual ConstructSet &relativecomplement(ConstructSet &) { return ExampleConstructSet(); }
	virtual ConstructSet &symmerticDifference(ConstructSet &) { return ExampleConstructSet(); }
};
#endif
}
}
}
#endif
