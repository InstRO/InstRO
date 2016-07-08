#include "instro/core/ConstructSet.h"

#include "instro/utility/Logger.h"

#include <assert.h>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iterator>

namespace InstRO {
namespace Core {
std::string Construct::specificConstructTraitToString() const {
	using CTLvl = InstRO::Core::ConstructTraitType;
	auto cts = getTraits();
	if (cts.is(CTLvl::CTOpenMPStatement)) {
		return "OpenMPStatement";
	}
	if (cts.is(CTLvl::CTConditionalStatement)) {
		return "ConditionalStatement";
	}
	if (cts.is(CTLvl::CTLoopStatement)) {
		return "LoopStatement";
	}
	if (cts.is(CTLvl::CTScopeStatement)) {
		return "ScopeStatement";
	}
	if (cts.is(CTLvl::CTSimpleStatement)) {
		return "SimpleStatement";
	}
	if (cts.is(CTLvl::CTWrappableStatement)) {
		return "WrappableStatement";
	}

	return InstRO::Core::toString(cts.max());
}

std::string ConstructTrait::toString() const {
	if (cts.empty()) {
		return InstRO::Core::toString(ConstructTraitType::CTNoTraits);
	}

	std::string str;
	str += "[";
	for (auto ct : cts) {
		str += InstRO::Core::toString(ct) + " ";
	}
	str += "]";
	return str;
}

std::string ConstructTrait::toStringShort() const {
	if (cts.empty()) {
		return InstRO::Core::constructLevelToStringShort(ConstructTraitType::CTNoTraits);
	}

	std::string str;
	str += "[";
	for (auto ct : cts) {
		str += InstRO::Core::constructLevelToStringShort(ct) + " ";
	}
	str += "]";
	return str;
}

ConstructTraitType ConstructSet::getMaxConstructLevel() const {
	ConstructTraitType max = ConstructTraitType::CTMin;
	for (auto construct : constructs) {
		auto curr = construct->getTraits().max();
		if (curr > max) {
			max = curr;
		}
	}
	return max;
}
ConstructTraitType ConstructSet::getMinConstructLevel() const {
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

// this will be in the protected developer interface
void ConstructSet::put(const std::shared_ptr<Construct>& construct) {
	if (construct->getTraits().is(ConstructTraitType::CTNoTraits)) {
		return;	// do not insert dummys
	}
	constructs.insert(construct);
}
void ConstructSet::erase(const std::shared_ptr<Construct>& construct) { constructs.erase(construct); }

void ConstructSet::put(ConstructSet cs) { constructs.insert(cs.begin(), cs.end()); }
void ConstructSet::erase(ConstructSet cs) { constructs.erase(cs.begin(), cs.end()); }

std::set<std::shared_ptr<Construct> >::iterator ConstructSet::begin() { return constructs.begin(); }
std::set<std::shared_ptr<Construct> >::iterator ConstructSet::end() { return constructs.end(); }
std::set<std::shared_ptr<Construct> >::const_iterator ConstructSet::begin() const { return constructs.cbegin(); }
std::set<std::shared_ptr<Construct> >::const_iterator ConstructSet::end() const { return constructs.cend(); }
std::set<std::shared_ptr<Construct> >::const_iterator ConstructSet::cbegin() const { return constructs.cbegin(); }
std::set<std::shared_ptr<Construct> >::const_iterator ConstructSet::cend() const { return constructs.cend(); }

bool ConstructSet::contains(const std::shared_ptr<Construct>& construct) const {
	if (constructs.find(construct) == constructs.end())
		return false;
	return true;
}

// https://en.wikipedia.org/wiki/Set_(mathematics)
ConstructSet ConstructSet::combine(const ConstructSet& other) const {
	ConstructSet retSet;

	std::set_union(
			begin(), end(), other.begin(), other.end(),
			std::inserter(retSet, retSet.begin()));

	return retSet;
}

ConstructSet ConstructSet::intersect(const ConstructSet& other) const {
	ConstructSet retSet;

	std::set_intersection(
			begin(), end(), other.begin(), other.end(),
			std::inserter(retSet, retSet.begin()));

	return retSet;
}

ConstructSet ConstructSet::relativeComplement(const ConstructSet& other) const {
	ConstructSet retSet;

	std::set_difference(
			begin(), end(), other.begin(), other.end(),
			std::inserter(retSet, retSet.begin()));

	return retSet;
}

ConstructSet ConstructSet::symmertricDifference(const ConstructSet& other) const {
	ConstructSet retSet;

	std::set_symmetric_difference(
			begin(), end(), other.begin(), other.end(),
			std::inserter(retSet, retSet.begin()));

	return retSet;
}

bool ConstructSet::intersects(const ConstructSet& other) const {
	return !(this->intersect(other).empty());
}

std::string ConstructSet::toString() const {
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

std::string ConstructSet::toDotString() const {
	std::string dotString;
	for (auto const& constructPtr : constructs) {
		dotString += constructPtr->toDotString();
		dotString += "\\n";
	}
	return dotString;
}

}	// namespace Core

namespace InfrastructureInterface {
ConstructSetCompilerInterface::ConstructSetCompilerInterface(Core::ConstructSet* pcs) : csPtr(pcs){}

void ConstructSetCompilerInterface::put(const std::shared_ptr<Core::Construct>& construct) { csPtr->put(construct); }
void ConstructSetCompilerInterface::erase(const std::shared_ptr<Core::Construct>& construct) {
	csPtr->erase(construct);
}
void ConstructSetCompilerInterface::put(Core::ConstructSet cs) { csPtr->put(cs); }
void ConstructSetCompilerInterface::erase(Core::ConstructSet cs) { csPtr->erase(cs); }
bool ConstructSetCompilerInterface::contains(const std::shared_ptr<Core::Construct>& construct) {
	return csPtr->contains(construct);
}

std::set<std::shared_ptr<Core::Construct> >::iterator ConstructSetCompilerInterface::begin() { return csPtr->begin(); }
std::set<std::shared_ptr<Core::Construct> >::iterator ConstructSetCompilerInterface::end() { return csPtr->end(); }
std::set<std::shared_ptr<Core::Construct> >::const_iterator ConstructSetCompilerInterface::cbegin() const {
	return csPtr->cbegin();
}
std::set<std::shared_ptr<Core::Construct> >::const_iterator ConstructSetCompilerInterface::cend() const {
	return csPtr->cend();
}

void ConstructSetCompilerInterface::clear() { csPtr->clear(); }
bool ConstructSetCompilerInterface::empty() { return csPtr->empty(); }
size_t ConstructSetCompilerInterface::size() { return csPtr->size(); }

ReadOnlyConstructSetCompilerInterface::ReadOnlyConstructSetCompilerInterface(const Core::ConstructSet* pcs)
		: csPtr(pcs){}

bool ReadOnlyConstructSetCompilerInterface::contains(const std::shared_ptr<Core::Construct>& construct) const {
	return csPtr->contains(construct);
}

std::set<std::shared_ptr<Core::Construct> >::const_iterator ReadOnlyConstructSetCompilerInterface::begin() const {
	return csPtr->cbegin();
}
std::set<std::shared_ptr<Core::Construct> >::const_iterator ReadOnlyConstructSetCompilerInterface::end() const {
	return csPtr->cend();
}

bool ReadOnlyConstructSetCompilerInterface::empty() const { return csPtr->empty(); }
size_t ReadOnlyConstructSetCompilerInterface::size() const { return csPtr->size(); }
}

}	// End namespace InstRO


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
		case InstRO::Core::ConstructTraitType::CTOpenMPStatement:
			os << "ConstructTraitType::CTOpenMPStatement";
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

namespace std {
bool less<std::shared_ptr<InstRO::Core::Construct> >::operator()(const std::shared_ptr<InstRO::Core::Construct>& a,
		const std::shared_ptr<InstRO::Core::Construct>& b) const {
	return a < b;
}
}
