#include "World.h"


void World::init(){
	//map.insert?
}

void World::insert(GameObject go){
	map.put(go.getId(), go);
}

void World::remove(int id){
	map.remove(id);
}

void World::replace(int id, GameObject go){
	this->remove(id);
	this->insert(go);
}

void World::get(int id){
	map.get(id);
}