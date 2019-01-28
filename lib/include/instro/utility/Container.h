#ifndef INSTRO_UTILITY_CONTAINER_H
#define INSTRO_UTILITY_CONTAINER_H

#include "instro/utility/exception.h"

#include <map>
#include <string>
#include <typeindex>
#include <typeinfo>

namespace InstRO {
namespace Utility {

template <typename T>
class Unpacker {
 public:
	typedef T* ret_type;
	Unpacker(std::intptr_t p) : d(p) {}
	ret_type operator()() { return reinterpret_cast<T*>(d); }

 private:
	std::intptr_t d;
};

/**
 * Offers support to store objects of different types in a look-up table.
 * The element is copied into the container's own storage and the container has ownership of its own copy.
 *
 * To retrieve the element, the user needs to provide the type. The cast however, will only succeed if the user
 * supplied actually the right type.
 *
 * The stored types must be CopyConstructible.
 */
class MultitypeMap {
 public:
	template <typename T>
	void insert(std::string id, T* obj) {
		T* temp = new T(*obj);	// we make our own copy
		size_t tHash = std::type_index(typeid(T)).hash_code();
		tMap[id] = tHash;
		objs[id] = reinterpret_cast<std::intptr_t>(temp);
	}

	template <typename T>
	void insert(std::string id, T&& obj) {
		T* temp = new T(std::forward<T>(obj));
		size_t tHash = std::type_index(typeid(T)).hash_code();
		tMap[id] = tHash;
		objs[id] = reinterpret_cast<std::intptr_t>(temp);
	}

	/**
	 * Searches the container for an element stored to the provided id.
	 * If found, the typeid is compared to the typeid of given type T.
	 *
	 * returns a T* to the stored element
	 * throws if typeids don't match
	 */
	template <typename T>
	T* get(std::string id) {
		auto i = tMap[id];
		if (i == std::type_index(typeid(T)).hash_code()) {
			return reinterpret_cast<T*>(objs[id]);
		}

		raise_exception("Could not create unpacker for requested type" + std::string(std::type_index(typeid(T)).name()));
	}

	~MultitypeMap() {
		objs.clear();
		tMap.clear();
	}

 private:
	std::map<std::string, std::intptr_t> objs;
	std::map<std::string, size_t> tMap;
};

}	// namespace Utility
}	// namespace InstRO
#endif
