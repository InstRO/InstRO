#ifndef INSTRO_CORE_CONSTRUCTSET_H
#define INSTRO_CORE_CONSTRUCTSET_H 0.1

#include <string>
#include <memory>	// Shared pointers
#include <vector>
#include <set>

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
}
namespace Core {

typedef enum ContstructLevelEnum {
	CLMin = 0,
	// Please do not use fragments. They may become deprecated
	CLFragment = 1,
	// Any expression with observable haviour
	CLExpression = 2,
	// separate Loop, Conditional, Scope and Simple Statments
	CLLoopStatement,
	CLConditionalStatement,
	CLScopeStatement,
	CLSimpleStatement,
	// a statement with observable behavior. No "pure" declarations, namespaces, classes, etc.
	CLStatement,
	// Wrappable statements
	CLWrappableStatement,
	CLFunction,
	CLFileScope,
	CLGlobalScope,
	CLMax,
	CLNotALevel
} ConstructLevelType;

std::string contstructLevelToString(ConstructLevelType type);
std::string operator+(const std::string& lhs, const ConstructLevelType& type);

/* CI: Construct Set implementation. Contribution by Roman Ness */
class Construct {
 public:
	virtual bool operator<(const Construct& b) { return false; }
	Construct() = delete;
	Construct(ConstructLevelType level) : construct_level(level){};
	ConstructLevelType getLevel() { return construct_level; }

	virtual ~Construct() {}

 protected:
	void setLevel(ConstructLevelType level) { construct_level = level; };
	ConstructLevelType construct_level;
};

/* CI: The ConstructSet class is intended to be specialized for each compiler
 * interface. It provides the basic mechanisms to specify what construct level
 * are contained.
 */
class ConstructSet {
	friend class InstRO::InfracstructureInterface::ConstructSetCompilerInterface;

 public:
	ConstructSet(){};
	void setCurrentMinLevel(ConstructLevelType minLevel){};
	void setCurrentMaxLevel(ConstructLevelType maxLevel){};

	// CI: return a vector (ordered) with all construct levels from the set
	virtual std::vector<ConstructLevelType> getConstructLevels();
	virtual ConstructLevelType getMaxConstructLevel();
	virtual ConstructLevelType getMinConstructLevel();
	virtual void clear();
	virtual bool empty();
	virtual size_t size();
	/*
	virtual void add(ConstructSet * setB) = NULL;
	virtual void add(ConstructSet & set) = NULL;*/
 protected:
	virtual void put(const std::shared_ptr<Construct>& construct);
	virtual void erase(const std::shared_ptr<Construct>& construct);
	virtual void put(ConstructSet cs);
	virtual void erase(ConstructSet cs);
	bool contains(const std::shared_ptr<Construct>& construct) const;

	ConstructSet(const std::shared_ptr<Construct>& construct) { constructs.insert(construct); };

	std::set<std::shared_ptr<Construct> >::iterator begin();
	std::set<std::shared_ptr<Construct> >::iterator end();
	std::set<std::shared_ptr<Construct> >::const_iterator cbegin() const;
	std::set<std::shared_ptr<Construct> >::const_iterator cend() const;

 public:
	// https://en.wikipedia.org/wiki/Set_(mathematics)
	// virtual unique_ptr<ConstructSet*> combine()
	virtual ConstructSet combine(const ConstructSet&) const;
	virtual ConstructSet intersect(const ConstructSet&) const;
	virtual ConstructSet relativecomplement(const ConstructSet&) const;
	virtual ConstructSet symmerticDifference(const ConstructSet&) const;
	// virtual ConstructSet copy(){ return  };
	virtual std::vector<ConstructSet> split() const;
	// CI: I would like to have s.th. like a begin() and end() returning an iterator of constructset containing individual
	// constructs

 protected:
	std::set<std::shared_ptr<Construct> > constructs;
};

}	// End Namespace Core

}	// End namespace InstRO
#endif
