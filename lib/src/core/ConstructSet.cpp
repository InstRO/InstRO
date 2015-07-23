#include <assert.h>
#include <vector>
#include <unordered_map>
#include "instro/core/ConstructSet.h"

namespace InstRO {
namespace Core {

std::string constructLevelToString(ConstructTraitType type) {
	switch (type) {
		case ConstructTraitType::CTFragment:
			return std::string("Fragment");
			break;
		case ConstructTraitType::CTExpression:
			return std::string("Expression");
			break;
		case ConstructTraitType::CTStatement:
			return std::string("Statement");
			break;
		case ConstructTraitType::CTWrappableStatement:
			return std::string("WrappableStatement");
			break;
		case ConstructTraitType::CTLoopStatement:
			return std::string("LoopStatement");
			break;
		case ConstructTraitType::CTConditionalStatement:
			return std::string("ConditionalStatement");
			break;
		case ConstructTraitType::CTScopeStatement:
			return std::string("ScopeStatement");
			break;
		case ConstructTraitType::CTSimpleStatement:
			return std::string("SimpleStatement");
			break;
		case ConstructTraitType::CTFunction:
			return std::string("Function");
			break;
		case ConstructTraitType::CTFileScope:
			return std::string("FileScope");
			break;
		case ConstructTraitType::CTGlobalScope:
			return std::string("GlobalScope");
			break;

		default:
			return std::string("Invalid ConstructTrait");
			break;
	}
}
std::string constructLevelToStringShort(ConstructTraitType type) {
	switch (type) {
		case ConstructTraitType::CTFragment:
			return std::string("Frag");
			break;
		case ConstructTraitType::CTExpression:
			return std::string("Expr");
			break;
		case ConstructTraitType::CTStatement:
			return std::string("Stmt");
			break;
		case ConstructTraitType::CTWrappableStatement:
			return std::string("WSmt");
			break;
		case ConstructTraitType::CTLoopStatement:
			return std::string("Loop");
			break;
		case ConstructTraitType::CTConditionalStatement:
			return std::string("Cond");
			break;
		case ConstructTraitType::CTScopeStatement:
			return std::string("ScopeStatement");
			break;
		case ConstructTraitType::CTSimpleStatement:
			return std::string("SSmt");
			break;
		case ConstructTraitType::CTFunction:
			return std::string("Func");
			break;
		case ConstructTraitType::CTFileScope:
			return std::string("File");
			break;
		case ConstructTraitType::CTGlobalScope:
			return std::string("Glob");
			break;

		default:
			return std::string("Invalid ConstructTrait");
			break;
	}
}

std::string operator+(const std::string& lhs, const ConstructTraitType& type) {
	return std::string(lhs).append(constructLevelToString(type));
}

ConstructTraitType ConstructSet::getMaxConstructLevel() {
	ConstructTraitType max = ConstructTraitType::CTMin;
	for (auto construct : constructs) {
		auto curr = construct->getTraits().max();
		if (curr > max) {
			max = curr;
		}
	}
	return max;
}
ConstructTraitType ConstructSet::getMinConstructLevel() {
	ConstructTraitType min = ConstructTraitType::CTMax;
	for (auto construct : constructs) {
		auto curr = construct->getTraits().min();
		if (curr < min) {
			min = curr;
		}
	}
	return min;
}
void ConstructSet::clear() { constructs.clear(); }
bool ConstructSet::empty() const { return constructs.empty(); }
size_t ConstructSet::size() const { return constructs.size(); }
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

std::vector<ConstructSet> ConstructSet::split() const {
	std::vector<ConstructSet> retVec;
	retVec.reserve(constructs.size());
	for (auto construct : constructs) {
		retVec.push_back(ConstructSet(construct));
	}
	return retVec;
}

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

ReadOnlyConstructSetCompilerInterface::ReadOnlyConstructSetCompilerInterface(const Core::ConstructSet* pcs)
		: csPtr(pcs){};

bool ReadOnlyConstructSetCompilerInterface::contains(const std::shared_ptr<Core::Construct>& construct) const {
	return csPtr->contains(construct);
};

std::set<std::shared_ptr<Core::Construct> >::const_iterator ReadOnlyConstructSetCompilerInterface::cbegin() const {
	return csPtr->cbegin();
};
std::set<std::shared_ptr<Core::Construct> >::const_iterator ReadOnlyConstructSetCompilerInterface::cend() const {
	return csPtr->cend();
};

bool ReadOnlyConstructSetCompilerInterface::empty() const { return csPtr->empty(); }
size_t ReadOnlyConstructSetCompilerInterface::size() const { return csPtr->size(); }
}

}	// End namespace InstRO

InstRO::Core::ConstructTraitType& operator++(InstRO::Core::ConstructTraitType& f) {
	InstRO::Core::ConstructLevelHelper::ConstructLevelHierarchy::raise(f);
	return f;
}

InstRO::Core::ConstructTraitType& operator--(InstRO::Core::ConstructTraitType& f) {
	InstRO::Core::ConstructLevelHelper::ConstructLevelHierarchy::lower(f);
	return f;
}

InstRO::Core::ConstructTraitType& operator++(InstRO::Core::ConstructTraitType& f, int f2) {
	InstRO::Core::ConstructLevelHelper::ConstructLevelHierarchy::raise(f);
	return f;
}

InstRO::Core::ConstructTraitType& operator--(InstRO::Core::ConstructTraitType& f, int f2) {
	InstRO::Core::ConstructLevelHelper::ConstructLevelHierarchy::lower(f);
	return f;
}

std::ostream& operator<<(std::ostream& os, InstRO::Core::ConstructTraitType f) {
	switch (f) {
		case InstRO::Core::ConstructTraitType::CTNoTraits:
			os << "ConstructTraitType::CTNoTraits";
			return os;
		case InstRO::Core::ConstructTraitType::CTMin:
			os << "ConstructTraitType::CTMin";
			return os;
		case InstRO::Core::ConstructTraitType::CTFragment:
			os << "ConstructTraitType::CTFragment";
			return os;
		case InstRO::Core::ConstructTraitType::CTExpression:
			os << "ConstructTraitType::CTExpression";
			return os;
		case InstRO::Core::ConstructTraitType::CTLoopStatement:
			os << "ConstructTraitType::CTLoopStatement";
			return os;
		case InstRO::Core::ConstructTraitType::CTConditionalStatement:
			os << "ConstructTraitType::CTConditionalStatement";
			return os;
		case InstRO::Core::ConstructTraitType::CTScopeStatement:
			os << "ConstructTraitType::CTScopeStatement";
			return os;
		case InstRO::Core::ConstructTraitType::CTSimpleStatement:
			os << "ConstructTraitType::CTSimpleStatement";
			return os;
		case InstRO::Core::ConstructTraitType::CTStatement:
			os << "ConstructTraitType::CTStatement";
			return os;

		case InstRO::Core::ConstructTraitType::CTWrappableStatement:
			os << "ConstructTraitType::CTWrappableStatement";
			return os;
		case InstRO::Core::ConstructTraitType::CTFunction:
			os << "ConstructTraitType::CTFunction";
			return os;
		case InstRO::Core::ConstructTraitType::CTFileScope:
			os << "ConstructTraitType::CTFileScope";
			return os;
		case InstRO::Core::ConstructTraitType::CTGlobalScope:
			os << "ConstructTraitType::CTGlobalScope";
			return os;
		case InstRO::Core::ConstructTraitType::CTMax:
			os << "ConstructTraitType::CTMax";
			return os;
	}
	return os;
}
