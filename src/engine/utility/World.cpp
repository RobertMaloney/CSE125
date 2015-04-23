#include "World.h"

void World::insert(GameObject go){
	map.put(go.getId(), go);
}

void