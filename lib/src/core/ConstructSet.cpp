#include <vector>
#include <unordered_map>
#include "instro/core/ConstructSet.h"

namespace InstRO{
namespace Core{

std::string constructLevelToString(ConstructLevelType type)
{
	switch(type)
	{
	// case 1: return std::string("LiteralConstructLevel");break;

	//case InstRO::Core::ContstructLevelEnum::CLMin:
	case InstRO::Core::ContstructLevelEnum::CLFragment:
		return std::string("CL-Fragment"); break;
	case CLExpression:
		return std::string("CL-Expression"); break;
	case CLStatement:
		return std::string("CL-Statement"); break;
	case CLLoop:
		return std::string("CL-LoopStatement"); break;
	case CLConditional:
		return std::string("CL-ConditionalStatement"); break;
	case CLScope:
		return std::string("CL-ScopeStatement"); break;
	case CLSimple:
		return std::string("CL-SimpleStatement"); break;
	case CLFunction:
		return std::string("CL-FunctionStatement"); break;
	case CLFileScope:
		return std::string("CL-FileScope"); break;
	// case CLMax:		
	case CLGlobalScope:
		return std::string("CL-GlobalScope"); break;
			
	default: return std::string("UnknownConstructLevel");break;
	}
}
std::string operator+(const std::string &lhs,const ConstructLevelType &type)
{
	return std::string(lhs).append(constructLevelToString(type));
	/*switch(type)
	{
	case 1: return std::string(lhs).append(std::string("LiteralConstructLevel"));break;
	default: return std::string(lhs).append(std::string("UnknownConstructLevel"));break;
	}*/
}

// CI: return a vector (ordered) with all construct levels from the set
::std::vector<ConstructLevelType> ConstructSet::getConstructLevels(){
	std::vector<int> levels;
	for (int i = ConstructLevelType::CLMin; i < ConstructLevelType::CLMax; i++)
		levels[i] = 0;
	for (auto construct : this->constructs)
	{
		levels[(*construct).getLevel()]++;
	}
	std::vector<ConstructLevelType> returnVector;
	for (int i = ConstructLevelType::CLMin; i < ConstructLevelType::CLMax; i++)
	{
		if (levels[i])
			returnVector.push_back(ConstructLevelType(i));
	}
	return returnVector;
}
ConstructLevelType ConstructSet::getMaxConstructLevel(){
	return ConstructSet::getConstructLevels().back();
}
ConstructLevelType ConstructSet::getMinConstructLevel(){
	return ConstructSet::getConstructLevels().front();
}
void ConstructSet::clear(){
	constructs.clear();
}
bool ConstructSet::empty(){
	return constructs.empty();
}
size_t ConstructSet::size(){
	return constructs.size();
}
/*
virtual void add(ConstructSet * setB) = NULL;
virtual void add(ConstructSet & set) = NULL;*/
//virtual ConstructSet intersect(ConstructSet b) = NULL;

// this will be in the protected developer interface
void ConstructSet::put(const std::shared_ptr<Construct>& construct){
	constructs.insert(construct);
}
void ConstructSet::erase(const std::shared_ptr<Construct>& construct){
	constructs.erase(construct);
}

void ConstructSet::put(ConstructSet cs){
	constructs.insert(cs.begin(), cs.end());
}
void ConstructSet::erase(ConstructSet cs){
	constructs.erase(cs.begin(),cs.end());
}


std::set<std::shared_ptr<Construct> >::iterator ConstructSet::begin(){
	return constructs.begin();
}
std::set<std::shared_ptr<Construct> >::iterator ConstructSet::end(){
	return constructs.end();
}
std::set<std::shared_ptr<Construct> >::const_iterator  ConstructSet::cbegin() const{
	return constructs.cbegin();
}
std::set<std::shared_ptr<Construct> >::const_iterator ConstructSet::cend() const{
	return constructs.cend();
}
bool ConstructSet::contains(const std::shared_ptr<Construct>& construct) const
{
	if (constructs.find(construct) == constructs.end()) return false;
	return true;
}

	// https://en.wikipedia.org/wiki/Set_(mathematics)
	//virtual unique_ptr<ConstructSet*> combine()
ConstructSet ConstructSet::combine(const ConstructSet & rhs) const{
	ConstructSet retSet;
	retSet.put(*this);
	retSet.put(rhs);
	return retSet;
}
ConstructSet ConstructSet::intersect(const ConstructSet & other) const{
	ConstructSet retSet;
	for (std::set<std::shared_ptr<Construct> >::const_iterator constructB = other.cbegin(); constructB != other.cend(); constructB++)
	{
		if (constructs.find(*constructB) == constructs.end())
			retSet.put(*constructB);
	}
	return retSet;
}
ConstructSet ConstructSet::relativecomplement(const ConstructSet & other) const{
	ConstructSet retSet;
	/* there may be a more efficient way of doing this, but it works*/
	for (auto  constructA : constructs)
	{
		if (!other.contains(constructA))
			retSet.put(constructA);
	}
	return retSet;
}
ConstructSet ConstructSet::symmerticDifference(const ConstructSet & other) const{
	ConstructSet retSet;
	for (std::set<std::shared_ptr<Construct> >::const_iterator constructB = other.cbegin(); constructB != other.cend(); constructB++)
	{
		if (constructs.find(*constructB) != constructs.end())
			retSet.put(*constructB);
	}
	return retSet;
}
	//virtual ConstructSet copy(){ return  };
::std::vector<ConstructSet> ConstructSet::split() const{
	std::vector<ConstructSet> retSet;
	retSet.reserve(constructs.size());
//	retSet.insert(constructs.begin(), constructs.end());
	for (auto construct : constructs)
	{
		retSet.push_back(ConstructSet(construct));
	}
	return retSet;
}
	// CI: I would like to have s.th. like a begin() and end() returning an iterator of constructset containing individual constructs





} // namespace Core
} // End namespace InstRO
