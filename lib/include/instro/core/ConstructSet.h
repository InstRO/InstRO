#ifndef INSTRO_CORE_CONSTRUCTSET_H
#define INSTRO_CORE_CONSTRUCTSET_H 0.1

#include <string>
#include <memory> // Shared pointers
// XXX Why do we want to use list here? I would prefer using vector!
//#include <list>
#include <vector>
#include <set>

namespace InstRO {

namespace Core {

typedef enum ContstructLevelEnum {
	CLMin = 0,
	CLFragment = 1,
	CLExpression = 2,
	CLStatement,
	CLLoop,
	CLConditional,
	CLScope,
	CLSimple,
	CLFunction,
	CLFileScope,
	CLGlobalScope,
	CLMax,
	CLNotALevel
} ConstructLevelType;

/*
class Construct
{
};
*/

std::string contstructLevelToString(ConstructLevelType type);
std::string operator+(const std::string &lhs, const ConstructLevelType &type);


/* CI: Construct Set implementation. Contribution by Roman Neﬂ */
class Construct{
public:
	virtual bool operator<(const  Construct & b){ return false; }
	Construct() = delete;
	Construct(ConstructLevelType level) :construct_level(level){};
	ConstructLevelType getLevel(){ return construct_level; }

protected:
	void setLevel(ConstructLevelType level){
		construct_level = level;
};
	ConstructLevelType construct_level;

};

/* CI: The ConstructSet class is intended to be specialized for each compiler
 * interface. It provides the basic mechanisms to specify what construct level
 * are contained. 
 */
class ConstructSet {
 public:
	ConstructSet(){};

	// CI: return a vector (ordered) with all construct levels from the set
	virtual ::std::vector<ConstructLevelType> getConstructLevels();
	virtual ConstructLevelType getMaxConstructLevel();
	virtual ConstructLevelType getMinConstructLevel();
	virtual void clear();
	virtual bool empty();
	virtual size_t size();
	/*
	virtual void add(ConstructSet * setB) = NULL;
	virtual void add(ConstructSet & set) = NULL;*/
	//virtual ConstructSet intersect(ConstructSet b) = NULL;

protected:	// this will be in the protected developer interface
	virtual void put(const std::shared_ptr<Construct>& construct);
	virtual void erase(const std::shared_ptr<Construct>& construct);
	virtual void put(ConstructSet cs);
	virtual void erase(ConstructSet cs);
	bool contains(const std::shared_ptr<Construct>& construct) const;
	ConstructSet(const std::shared_ptr<Construct>& construct){
		constructs.insert(construct);
	};


	std::set<std::shared_ptr<Construct> >::iterator begin();
	std::set<std::shared_ptr<Construct> >::iterator end();
	std::set<std::shared_ptr<Construct> >::const_iterator  cbegin()const;
	std::set<std::shared_ptr<Construct> >::const_iterator  cend()const;
public:
	// https://en.wikipedia.org/wiki/Set_(mathematics)
	//virtual unique_ptr<ConstructSet*> combine()
	virtual ConstructSet combine(const ConstructSet &) const;
	virtual ConstructSet intersect(const ConstructSet &) const ;
	virtual ConstructSet relativecomplement(const ConstructSet &) const;
	virtual ConstructSet symmerticDifference(const ConstructSet &) const;
	//virtual ConstructSet copy(){ return  };
	virtual ::std::vector<ConstructSet> split() const;
	// CI: I would like to have s.th. like a begin() and end() returning an iterator of constructset containing individual constructs

private:

	std::set<std::shared_ptr<Construct> > constructs;
};

} // End Namespace Core
} // End namespace InstRO
#endif
