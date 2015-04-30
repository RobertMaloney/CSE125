#include "World.h"


void World::init(){
	//map.insert?
}

void World::insert(GameObject * go){
	if (!(go->getId())){
		//TODO Exception
	}
	map.add(go->getId(), go);
}

void World::remove(ObjectId id){
	map.remove(id);
}

void World::replace(ObjectId id, GameObject * go){
	this->remove(id);
	this->insert(go);
}

void World::get(ObjectId id){
	map.get(id);
}