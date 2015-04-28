#ifndef WORLD_H
#define WORLD_H

#include "EntityMap.h"
#include "GameObject.h"

class World {

protected:
	//a list of players?
	//a list of resources?
	//a list of NPCs?
	//a map of everything?
	//a map of resources?
	EntityMap<ObjectId, GameObject>  map;
public:
	World();
	virtual ~World();

	void init();
	void insert(GameObject go);
	void remove(ObjectId id);
	void replace(ObjectId id, GameObject go);
	void get(ObjectId id);

};
#endif