#ifndef ENTITY_MAP_H
#define ENTITY_MAP_H

using namespace std;

template<typename K, typename V>
class EntityMap {

protected:
	unordered_map<K, V> emap;

public:
	
	EntityMap();
	virtual ~EntityMap();

	int size();

	int put(K, V);
	V get(K);
	int remove(K);

	void clear();
};

#endif // ENTITY_MAP_H
