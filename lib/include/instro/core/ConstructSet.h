#ifndef INSTRO_CORE_CONSTRUCTSET_H
#define INSTRO_CORE_CONSTRUCTSET_H 0.1

#include <string>
#include <sstream>
#include <memory>	// Shared pointers
#include <vector>
#include <set>

#include <bitset>

namespace InstRO {

namespace Core {
class Construct;
class ConstructSet;
}

namespace InfracstructureInterface {
class ConstructSetCompilerInterface {
 protected:
	Core::ConstructSet* csPtr;

 public:
	ConstructSetCompilerInterface() = delete;
	ConstructSetCompilerInterface(Core::ConstructSet* pcs);

	void put(const std::shared_ptr<Core::Construct>& construct);
	void erase(const std::shared_ptr<Core::Construct>& construct);
	void put(Core::ConstructSet cs);
	void erase(Core::ConstructSet cs);
	bool contains(const std::shared_ptr<Core::Construct>& construct);

	std::set<std::shared_ptr<Core::Construct> >::iterator begin();
	std::set<std::shared_ptr<Core::Construct> >::iterator end();
	std::set<std::shared_ptr<Core::Construct> >::const_iterator cbegin() const;
	std::set<std::shared_ptr<Core::Construct> >::const_iterator cend() const;
	void clear();
	bool empty();
	size_t size();
};
}	// namespace InfracstructureInterface

namespace Core {

typedef enum ContstructTraitEnum {
	CTNoTraits			  =	 0,	// TODO this should no longer be necessary?
	CTMin                 = 1,
	// Please do not use fragments. They may become deprecated
	CTFragment            = 2,
	// Any expression with observable behavior
	CTExpression          = 3,
	// separate Loop, Conditional, Scope and Simple Statements
	CTLoopStatement       = 4,
	CTConditionalStatement= 5,
	CTScopeStatement      = 6,
	CTSimpleStatement     = 7,
	// a statement with observable behavior. No "pure" declarations, namespaces, classes, etc.
	CTStatement           = 8,
	// Wrappable statements
	CTWrappableStatement  = 9,
	CTFunction            = 10,
	CTFileScope           = 11,
	CTGlobalScope         = 12,
	CTMax                 = 13
} ConstructTraitType;

std::string constructLevelToString(ConstructTraitType type);
std::string operator+(const std::string& lhs, const ConstructTraitType& type);

class ConstructTrait {
public:
	ConstructTrait() = delete;

/*	template <class... TraitList>
	ChannelConfiguration(Pass *p1, PassList... passes) {
		inputChannelPasses.insert(inputChannelPasses.begin(), { p1, passes... });
	}*/

	template <class... TraitList> ConstructTrait(TraitList... traits){
		cts.insert({ traits... });
	}

	ConstructTrait(ConstructTraitType type) {
		add(type);
	}

	bool is(ConstructTraitType type) {
		if (cts.empty()) {
			return type==CTNoTraits;
		}
		return cts.find(type) != cts.end();
	}
	void add(ConstructTraitType type) {
		cts.insert(type);
	}
	ConstructTraitType max() {
		return *cts.crbegin();
	}
	ConstructTraitType min() {
		return *cts.cbegin();
	}
	const std::set<ConstructTraitType>& getTraitsAsSet() {
		return cts;
	}

	std::string toString() {
		if (cts.empty()) {
			return InstRO::Core::constructLevelToString(CTNoTraits);
		}

		std::stringstream ss;
		ss << "[";
		for (auto ct : cts) {
			ss << InstRO::Core::constructLevelToString(ct) << " ";
		}
		ss << "]";
		return ss.str();
	}

private:
	std::set<ConstructTraitType> cts;
};



/* CI: Construct Set implementation. Contribution by Roman Ness */
class Construct {
 public:
	virtual bool operator<(const Construct& b) { return false; }
	Construct(ConstructTrait traits):constructTraits(traits){};
	ConstructTrait getTraits() { return constructTraits; }
	const std::set<ConstructTraitType>& getTraitsAsSet() {
		return constructTraits.getTraitsAsSet();
	}

	virtual ~Construct() {}

 protected:
	ConstructTrait constructTraits;
};

/* CI: The ConstructSet class is intended to be specialized for each compiler
 * interface. It provides the basic mechanisms to specify what construct level
 * are contained.
 */
class ConstructSet {
	friend class InstRO::InfracstructureInterface::ConstructSetCompilerInterface;

 public:
	ConstructSet(){};
	void setCurrentMinLevel(ConstructTraitType minLevel){};
	void setCurrentMaxLevel(ConstructTraitType maxLevel){};

	virtual ConstructTraitType getMaxConstructLevel();
	virtual ConstructTraitType getMinConstructLevel();
	virtual void clear();
	virtual bool empty();
	virtual size_t size();

 protected:
	ConstructSet(const std::shared_ptr<Construct>& construct) { constructs.insert(construct); };

	virtual void put(const std::shared_ptr<Construct>& construct);
	virtual void erase(const std::shared_ptr<Construct>& construct);
	virtual void put(ConstructSet cs);
	virtual void erase(ConstructSet cs);
	bool contains(const std::shared_ptr<Construct>& construct) const;

	std::set<std::shared_ptr<Construct> >::iterator begin();
	std::set<std::shared_ptr<Construct> >::iterator end();
	std::set<std::shared_ptr<Construct> >::const_iterator cbegin() const;
	std::set<std::shared_ptr<Construct> >::const_iterator cend() const;

 public:
	// https://en.wikipedia.org/wiki/Set_(mathematics)
	virtual ConstructSet combine(const ConstructSet&) const;
	virtual ConstructSet intersect(const ConstructSet&) const;
	virtual ConstructSet relativecomplement(const ConstructSet&) const;
	virtual ConstructSet symmerticDifference(const ConstructSet&) const;

	virtual std::vector<ConstructSet> split() const;
	// CI: I would like to have s.th. like a begin() and end() returning an iterator of constructset containing individual
	// constructs

 protected:
	std::set<std::shared_ptr<Construct> > constructs;
};

}	// End Namespace Core
}	// End namespace InstRO

#endif
