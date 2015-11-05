#ifndef INSTRO_CORE_CONSTRUCTSET_H
#define INSTRO_CORE_CONSTRUCTSET_H

#include <string>
#include <sstream>
#include <memory>	// Shared pointers
#include <vector>
#include <set>

namespace InstRO {

namespace Core {
class Construct;
class ConstructSet;
}

namespace InfrastructureInterface {
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

	std::set<std::shared_ptr<Core::Construct> >::const_iterator begin() const;
	std::set<std::shared_ptr<Core::Construct> >::const_iterator end() const;
	bool empty() const;
	size_t size() const;
};
}	// namespace InfrastructureInterface

namespace Core {

enum class ConstructTraitType {
	CTNoTraits = 0,	// TODO this should no longer be necessary?
	CTMin = 1,
	// Please do not use fragments. They may become deprecated
	CTFragment = 2,
	// Any expression with observable behavior
	CTExpression = 3,
	CTStatement = 4,
	// separate Loop, Conditional, Scope and Simple Statements
	CTLoopStatement = 5,
	CTConditionalStatement = 6,
	CTScopeStatement = 7,
	CTSimpleStatement = 8,
	// a statement with observable behavior. No "pure" declarations, namespaces, classes, etc.
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
		ConstructTraitType::CTStatement, ConstructTraitType::CTLoopStatement, ConstructTraitType::CTConditionalStatement,
		ConstructTraitType::CTScopeStatement, ConstructTraitType::CTSimpleStatement,
		// a statement with observable behavior. No "pure" declarations, namespaces, classes, etc.
		// Wrappable statements
		ConstructTraitType::CTWrappableStatement, ConstructTraitType::CTFunction, ConstructTraitType::CTFileScope,
		ConstructTraitType::CTGlobalScope, ConstructTraitType::CTMax> ConstructLevelHierarchy;
}

std::string constructLevelToString(ConstructTraitType type);
std::string constructLevelToStringShort(ConstructTraitType type);
std::string operator+(const std::string& lhs, const ConstructTraitType& type);

class ConstructTrait {
 public:
	bool operator==(const ConstructTraitType& comparator) {
		if (cts.find(comparator) != cts.end())
			return true;
		return false;
	}
	ConstructTrait() = delete;

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
	std::string toStringShort() {
		if (cts.empty()) {
			return InstRO::Core::constructLevelToString(ConstructTraitType::CTNoTraits);
		}

		std::stringstream ss;
		ss << "[";
		for (auto ct : cts) {
			ss << InstRO::Core::constructLevelToStringShort(ct) << " ";
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

	ConstructTrait getTraits() const { return constructTraits; }

	const std::set<ConstructTraitType>& getTraitsAsSet() { return constructTraits.getTraitsAsSet(); }

	virtual size_t getID() const = 0;
	virtual std::string toString() const { return std::string("Construct(abstract)"); }
	virtual std::string toDotString() const = 0;
	virtual std::string getIdentifier() const = 0;



 protected:
	ConstructTrait constructTraits;

	friend bool __attribute__ ((always_inline)) operator<(const std::shared_ptr<Construct>& a, const std::shared_ptr<Construct>& b) {

		// FIXME RN 2015-11: For some supernatural reason the operator does not work correctly without the following statement.
		b->toString();

		return a->getID() < b->getID();
	}

//	friend bool operator==(const std::shared_ptr<Construct>& a, const std::shared_ptr<Construct>& b) { return a->getID() == b->getID(); }
};

class ConstructSet {
	friend class InstRO::InfrastructureInterface::ConstructSetCompilerInterface;
	friend class InstRO::InfrastructureInterface::ReadOnlyConstructSetCompilerInterface;
	friend class std::insert_iterator<ConstructSet>;

 public:
	typedef std::shared_ptr<Construct> value_type;
	typedef std::set<value_type>::iterator iterator;
	typedef std::set<value_type>::const_iterator const_iterator;

	ConstructSet(){};

	ConstructTraitType getMaxConstructLevel();
	ConstructTraitType getMinConstructLevel();
	void clear();
	bool empty() const;
	size_t size() const;

	ConstructSet(const value_type& construct) { constructs.insert(construct); };
 protected:
	iterator insert(iterator it, const value_type& val) {
		return constructs.insert(it, val);
	}

 protected:
	void put(const value_type& construct);
	void erase(const value_type& construct);
	void put(ConstructSet cs);
	void erase(ConstructSet cs);
	bool contains(const value_type& construct) const;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;

 public:
	// https://en.wikipedia.org/wiki/Set_(mathematics)
	ConstructSet combine(const ConstructSet&) const;
	ConstructSet intersect(const ConstructSet&) const;
	ConstructSet relativeComplement(const ConstructSet&) const;
	ConstructSet symmertricDifference(const ConstructSet&) const;

	bool intersects(const ConstructSet&) const;

	std::vector<ConstructSet> split() const;

	std::string toString() const {
        std::string str;
        auto constructIter = begin();
        while (constructIter != end()) {
            auto currentIter = constructIter++;
            auto constructPtr = *currentIter;
            str += constructPtr->toString();
            if (constructIter != end()) {
                str += ", ";
            }
        }
        return str;
	}

	std::string toDotString() const {
		std::string dotString;
		for (auto const& constructPtr : constructs) {
			dotString += constructPtr->toDotString();
			dotString += "\\n";
		}
		return dotString;
	}

 protected:
	std::set<value_type> constructs;

	friend bool operator<(const ConstructSet& c1, const ConstructSet& c2) { return c1.constructs < c2.constructs; }
//	friend bool operator==(const ConstructSet& c1, const ConstructSet& c2) { return c1.constructs == c2.constructs; }

	friend std::ostream& operator<<(std::ostream& out, const ConstructSet& cs) {
		out << " CS size:" << cs.size() << std::endl;
		for (auto const& constructPtr : cs) {
			out << "\t" << constructPtr->toString() << std::endl;
		}
		return out;
	}
};


template<typename T>
struct ConstructHasher {
	size_t operator()(T const &c){
		return c.getID();
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
