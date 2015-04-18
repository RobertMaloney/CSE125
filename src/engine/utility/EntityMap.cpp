#include <unordered_map>
#include "EntityMap.h"

template<typename K, typename V>
int EntityMap<K,V>::size() {
	return emap.size();
}

template<typename K, typename V>
int EntityMap<K,V>::put(K, V) {
	emap[K] = V;
}

template<typename K, typename V>
V EntityMap<K,V>::get(K) {
	return emap[K];
}

template<typename K, typename V>
int EntityMap<K,V>::remove(K) {
	return emap.erase(K);
};

template<typename K, typename V>
void EntityMap<K,V>::clear(){
	emap.clear();
}