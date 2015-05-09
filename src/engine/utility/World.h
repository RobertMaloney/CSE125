#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>

#include "ObjectDB.h"

using std::hash;
using std::pair;
using std::unordered_map;

class World {

public:
	
	World();
	~World();

	void update(float dt);

private:

	void checkCollision(GameObject* ob1, GameObject* ob2);

	ObjectDB* objectDb;
};

#endif