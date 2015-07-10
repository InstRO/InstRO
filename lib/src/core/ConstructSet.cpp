#include <assert.h>
#include <vector>
#include <unordered_map>
#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Core {

std::string constructLevelToString(ConstructTraitType type) {
	switch (type) {
		case CTFragment:
			return std::string("Fragment");
			break;
		case CTExpression:
			return std::string("Expression");
			break;
		case CTStatement:
			return std::string("Statement");
			break;
		case CTWrappableStatement:
			return std::string("WrappableStatement");
			break;
		case CTLoopStatement:
			return std::string("LoopStatement");
			break;
		case CTConditionalStatement:
			return std::string("ConditionalStatement");
			break;
		case CTScopeStatement:
			return std::string("ScopeStatement");
			break;
		case CTSimpleStatement:
			return std::string("SimpleStatement");
			break;
		case CTFunction:
			return std::string("Function");
			break;
		case CTFileScope:
			return std::string("FileScope");
			break;
		case CTGlobalScope:
			return std::string("GlobalScope");
			break;

		default:
			return std::string("Invalid ConstructTrait");
			break;
	}
}

std::string operator+(const std::string& lhs, const ConstructTraitType& type) {
	return std::string(lhs).append(constructLevelToString(type));
}

// CI: return a vector (ordered) with all construct levels from the set
std::vector<ConstructTraitType> ConstructSet::getConstructLevels() {
	std::vector<int> levels;
	// set the counter for all available levels to 0
	for (int i = ConstructTraitType::CTMin; i < ConstructTraitType::CTMax; i++)
		levels.push_back(0);
	// for each construct, determine its level and increment the corresponding bucket
	for (auto construct : constructs) {
		assert(construct->getLevel() > ContstructTraitEnum::CTMin);
		assert(construct->getLevel() < ContstructTraitEnum::CTMax);

		levels[construct->getLevel()]++;
	}
	std::vector<ConstructTraitType> returnVector;
	for (int i = ConstructTraitType::CTMin; i < ConstructTraitType::CTMax; i++) {
		if (levels[i])
			returnVector.push_back(ConstructTraitType(i));
	}
	return returnVector;
}
ConstructTraitType ConstructSet::getMaxConstructLevel() {
	std::vector<ConstructTraitType> levels = ConstructSet::getConstructLevels();
	if (levels.empty())
		return ContstructTraitEnum::CTMax;
	else
		return levels.back();
}
ConstructTraitType ConstructSet::getMinConstructLevel() {
	std::vector<ConstructTraitType> levels = ConstructSet::getConstructLevels();
	if (levels.empty())
		return ContstructTraitEnum::CTMin;
	else
		return levels.front();
	return ConstructSet::getConstructLevels().front();
}
void ConstructSet::clear() { constructs.clear(); }
bool ConstructSet::empty() { return constructs.empty(); }
size_t ConstructSet::size() { return constructs.size(); }
/*
virtual void add(ConstructSet * setB) = NULL;
virtual void add(ConstructSet & set) = NULL;*/
// virtual ConstructSet intersect(ConstructSet b) = NULL;

// this will be in the protected developer interface
void ConstructSet::put(const std::shared_ptr<Construct>& construct) { constructs.insert(construct); }
void ConstructSet::erase(const std::shared_ptr<Construct>& construct) { constructs.erase(construct); }

void ConstructSet::put(ConstructSet cs) { constructs.insert(cs.begin(), cs.end()); }
void ConstructSet::erase(ConstructSet cs) { constructs.erase(cs.begin(), cs.end()); }

std::set<std::shared_ptr<Construct> >::iterator ConstructSet::begin() { return constructs.begin(); }
std::set<std::shared_ptr<Construct> >::iterator ConstructSet::end() { return constructs.end(); }
std::set<std::shared_ptr<Construct> >::const_iterator ConstructSet::cbegin() const { return constructs.cbegin(); }
std::set<std::shared_ptr<Construct> >::const_iterator ConstructSet::cend() const { return constructs.cend(); }
bool ConstructSet::contains(const std::shared_ptr<Construct>& construct) const {
	if (constructs.find(construct) == constructs.end())
		return false;
	return true;
}

// https://en.wikipedia.org/wiki/Set_(mathematics)
// virtual unique_ptr<ConstructSet*> combine()
ConstructSet ConstructSet::combine(const ConstructSet& rhs) const {
	ConstructSet retSet;
	retSet.put(*this);
	retSet.put(rhs);
	return retSet;
}

ConstructSet ConstructSet::intersect(const ConstructSet& other) const {
	ConstructSet retSet;
	for (std::set<std::shared_ptr<Construct> >::const_iterator constructB = other.cbegin(); constructB != other.cend();
			 constructB++) {
		// if there is a match between both sets, add it to the output set
		if (constructs.find(*constructB) != constructs.end())
			retSet.put(*constructB);
	}
	return retSet;
}
ConstructSet ConstructSet::relativecomplement(const ConstructSet& other) const {
	ConstructSet retSet;
	/* there may be a more efficient way of doing this, but it works*/
	for (auto constructA : constructs) {
		if (!other.contains(constructA))
			retSet.put(constructA);
	}
	return retSet;
}
ConstructSet ConstructSet::symmerticDifference(const ConstructSet& other) const {
	ConstructSet retSet;
	for (std::set<std::shared_ptr<Construct> >::const_iterator constructB = other.cbegin(); constructB != other.cend();
			 constructB++) {
		if (constructs.find(*constructB) != constructs.end())
			retSet.put(*constructB);
	}
	return retSet;
}
// virtual ConstructSet copy(){ return  };
::std::vector<ConstructSet> ConstructSet::split() const {
	std::vector<ConstructSet> retSet;
	retSet.reserve(constructs.size());
	//	retSet.insert(constructs.begin(), constructs.end());
	for (auto construct : constructs) {
		retSet.push_back(ConstructSet(construct));
	}
	return retSet;
}
// CI: I would like to have s.th. like a begin() and end() returning an iterator of constructset containing individual
// constructs

}	// namespace Core

namespace InfracstructureInterface {
ConstructSetCompilerInterface::ConstructSetCompilerInterface(Core::ConstructSet* pcs) : csPtr(pcs){};

void ConstructSetCompilerInterface::put(const std::shared_ptr<Core::Construct>& construct) { csPtr->put(construct); };
void ConstructSetCompilerInterface::erase(const std::shared_ptr<Core::Construct>& construct) {
	csPtr->erase(construct);
};
void ConstructSetCompilerInterface::put(Core::ConstructSet cs) { csPtr->put(cs); };
void ConstructSetCompilerInterface::erase(Core::ConstructSet cs) { csPtr->erase(cs); };
bool ConstructSetCompilerInterface::contains(const std::shared_ptr<Core::Construct>& construct) {
	return csPtr->contains(construct);
};

std::set<std::shared_ptr<Core::Construct> >::iterator ConstructSetCompilerInterface::begin() { return csPtr->begin(); };
std::set<std::shared_ptr<Core::Construct> >::iterator ConstructSetCompilerInterface::end() { return csPtr->end(); };
std::set<std::shared_ptr<Core::Construct> >::const_iterator ConstructSetCompilerInterface::cbegin() const {
	return csPtr->cbegin();
};
std::set<std::shared_ptr<Core::Construct> >::const_iterator ConstructSetCompilerInterface::cend() const {
	return csPtr->cend();
};

void ConstructSetCompilerInterface::clear() { csPtr->clear(); }
bool ConstructSetCompilerInterface::empty() { return csPtr->empty(); }
size_t ConstructSetCompilerInterface::size() { return csPtr->size(); }
}

}	// End namespace InstRO
