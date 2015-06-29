#ifndef INSTRO_CORE_CONSTRUCTSET_H
#define INSTRO_CORE_CONSTRUCTSET_H 0.1

#include <string>
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
	CLMax
} ConstructLevelType;

/*
class Construct
{
};
*/

std::string contstructLevelToString(ConstructLevelType type);
std::string operator+(const std::string &lhs, const ConstructLevelType &type);


/* CI: The ConstructSet class is intended to be specialized for each compiler
 * interface. It provides the basic mechanisms to specify what construct level
 * are contained. 
 */
class ConstructSet {
 public:
	ConstructSet(){};

	// CI: return a vector (ordered) with all construct levels from the set
	virtual ::std::vector<ConstructLevelType> getConstructLevels() = NULL;
	virtual ConstructLevelType getMaxConstructLevel()=NULL;
	virtual ConstructLevelType getMinConstructLevel()=NULL;
	virtual void clear()=NULL;
	virtual size_t size() = NULL;
	/*
	virtual void add(ConstructSet * setB) = NULL;
	virtual void add(ConstructSet & set) = NULL;*/
	//virtual ConstructSet intersect(ConstructSet b) = NULL;
	
	virtual void put(ConstructSet &) = NULL;
	virtual void erase(ConstructSet &) = NULL;

	// https://en.wikipedia.org/wiki/Set_(mathematics)
	//virtual unique_ptr<ConstructSet*> combine()
	virtual ConstructSet & combine(ConstructSet &) = NULL;
	virtual ConstructSet & intersect(ConstructSet &) = NULL;
	virtual ConstructSet & relativecomplement(ConstructSet &) = NULL;
	virtual ConstructSet & symmerticDifference(ConstructSet&) = NULL;
	virtual ConstructSet * copy(){ return this; };
	//virtual ::std::set<ConstructSet &> split() = NULL;
	// CI: I would like to have s.th. like a begin() and end() returning an iterator of constructset containing individual constructs
};

} // End Namespace Core
} // End namespace InstRO
#endif
