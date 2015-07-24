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

class ReadOnlyConstructSetCompilerInterface {
 protected:
	const Core::ConstructSet* csPtr;

 public:
	ReadOnlyConstructSetCompilerInterface() = delete;
	ReadOnlyConstructSetCompilerInterface(const Core::ConstructSet* pcs);

	bool contains(const std::shared_ptr<Core::Construct>& construct) const;

	std::set<std::shared_ptr<Core::Construct> >::const_iterator cbegin() const;
	std::set<std::shared_ptr<Core::Construct> >::const_iterator cend() const;
	bool empty() const;
	size_t size() const;
};
}	// namespace InfracstructureInterface

namespace Core {

/*
typedef enum ContstructTraitEnum {...
} ConstructTraitType;
*/

enum class ConstructTraitType {
	CTNoTraits = 0,	// TODO this should no longer be necessary?
	CTMin = 1,
	// Please do not use fragments. They may become deprecated
	CTFragment = 2,
	// Any expression with observable behavior
	CTExpression = 3,
	// separate Loop, Conditional, Scope and Simple Statements
	CTLoopStatement = 4,
	CTConditionalStatement = 5,
	CTScopeStatement = 6,
	CTSimpleStatement = 7,
	// a statement with observable behavior. No "pure" declarations, namespaces, classes, etc.
	CTStatement = 8,
	// Wrappable statements
	CTWrappableStatement = 9,
	CTFunction = 10,
	CTFileScope = 11,
	CTGlobalScope = 12,
	CTMax = 13
};

// Derived from  "Advanced Enums  -  http://ideone.com/Htlg0G"
namespace ConstructLevelHelper {
template <typename E, E first>
void raiseEnum(E& v) {
	// If this is the last element in the construct level hierarchy, we can not raise anymore
}

template <typename E, E head, E next, E... tail>
void raiseEnum(E& v) {
	// if the current head matches, elevate to the next construct level
	if (v == head)
		v = next;
	else
		// check the next element in the list
		raiseEnum<E, next, tail...>(v);
}

template <typename E, E first>
void lowerEnum(E& v) {
	// if the current construct level is the max level, lowing it means to go to the last element in the list
}

template <typename E, E head, E next, E... tail>
void lowerEnum(E& v) {
	if (v == next)
		v = head;
	else
		lowerEnum<E, next, tail...>(v);
}

template <typename E, E min, E first, E second, E... values>
struct ConstructTraitHierarchyTraverser {
	static void raise(E& v) {
		if (v == min)
			v = first;
		raiseEnum<E, min, first, second, values...>(v);
	}
	static void lower(E& v) { lowerEnum<E, min, first, second, values...>(v); }
};

/// Scalable way, C++11-ish
typedef ConstructTraitHierarchyTraverser<
		ConstructTraitType, ConstructTraitType::CTMin, ConstructTraitType::CTFragment, ConstructTraitType::CTExpression,
		ConstructTraitType::CTLoopStatement, ConstructTraitType::CTConditionalStatement,
		ConstructTraitType::CTScopeStatement, ConstructTraitType::CTSimpleStatement,
		// a statement with observable behavior. No "pure" declarations, namespaces, classes, etc.
		ConstructTraitType::CTStatement,
		// Wrappable statements
		ConstructTraitType::CTWrappableStatement, ConstructTraitType::CTFunction, ConstructTraitType::CTFileScope,
		ConstructTraitType::CTGlobalScope, ConstructTraitType::CTMax> ConstructLevelHierarchy;
}

std::string constructLevelToString(ConstructTraitType type);
std::string operator+(const std::string& lhs, const ConstructTraitType& type);

class ConstructTrait {
 public:
	ConstructTrait() = delete;

	/*	template <class... TraitList>
		ChannelConfiguration(Pass *p1, PassList... passes) {
			inputChannelPasses.insert(inputChannelPasses.begin(), { p1, passes... });
		}*/

	template <class... TraitList>
	ConstructTrait(TraitList... traits) {
		cts.insert({traits...});
	}

	ConstructTrait(ConstructTraitType type) { add(type); }

	bool is(ConstructTraitType type) {
		if (cts.empty()) {
			return type == ConstructTraitType::CTNoTraits;
		}
		return cts.find(type) != cts.end();
	}

	void add(ConstructTraitType type) { cts.insert(type); }
	ConstructTraitType max() { return *cts.crbegin(); }
	ConstructTraitType min() { return *cts.cbegin(); }
	const std::set<ConstructTraitType>& getTraitsAsSet() { return cts; }

	std::string toString() {
		if (cts.empty()) {
			return InstRO::Core::constructLevelToString(ConstructTraitType::CTNoTraits);
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
	Construct(ConstructTrait traits) : constructTraits(traits) {}
	virtual ~Construct() {}

	ConstructTrait getTraits() { return constructTraits; }

	const std::set<ConstructTraitType>& getTraitsAsSet() { return constructTraits.getTraitsAsSet(); }

	bool operator<(const Construct& other) { return getID() < other.getID(); }
	bool operator==(const Construct& other) { return getID() == other.getID(); }

	virtual size_t getID() const = 0;
	virtual std::string toString() { return std::string("Construct(abstract)"); }

 protected:
	ConstructTrait constructTraits;
};

/* CI: The ConstructSet class is intended to be specialized for each compiler
 * interface. It provides the basic mechanisms to specify what construct level
 * are contained.
 */
class ConstructSet {
	friend class InstRO::InfracstructureInterface::ConstructSetCompilerInterface;
	friend class InstRO::InfracstructureInterface::ReadOnlyConstructSetCompilerInterface;

 public:
	ConstructSet(){};
	// XXX RN: in the long run there should be no child classes from ConstructSet
	virtual ~ConstructSet() {};

	void setCurrentMinLevel(ConstructTraitType minLevel){};
	void setCurrentMaxLevel(ConstructTraitType maxLevel){};

	virtual ConstructTraitType getMaxConstructLevel();
	virtual ConstructTraitType getMinConstructLevel();
	virtual void clear();
	bool empty() const;
	size_t size() const;

	ConstructSet(const std::shared_ptr<Construct>& construct) { constructs.insert(construct); };

 protected:
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

	friend bool operator<(const ConstructSet& c1, const ConstructSet& c2) { return c1.constructs < c2.constructs; }
	friend bool operator==(const ConstructSet& c1, const ConstructSet& c2) { return c1.constructs == c2.constructs; }

	friend std::ostream& operator<<(std::ostream& out, ConstructSet& cs) {
		out << " CS size:" << cs.size() << std::endl;
		InfracstructureInterface::ConstructSetCompilerInterface csci(&cs);
		for (auto const& constructPtr : cs) {
			out << "\t" << constructPtr->toString() << std::endl;
		}
		return out;
	}
};

}	// End Namespace Core
}	// End namespace InstRO

InstRO::Core::ConstructTraitType& operator++(InstRO::Core::ConstructTraitType& f);
InstRO::Core::ConstructTraitType& operator--(InstRO::Core::ConstructTraitType& f);
InstRO::Core::ConstructTraitType& operator++(InstRO::Core::ConstructTraitType& f, int f2);
InstRO::Core::ConstructTraitType& operator--(InstRO::Core::ConstructTraitType& f, int f2);
std::ostream& operator<<(std::ostream& os, InstRO::Core::ConstructTraitType f);

#endif
