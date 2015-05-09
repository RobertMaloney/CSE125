#include "World.h"


World::World() {
	objectDb = &ObjectDB::getInstance();
}


World::~World() {

}


void World::update(float dt) {
	for_each(objectDb->objects.begin(), objectDb->objects.end(),
		[&] (pair<const ObjectId, GameObject*> pair) -> void {
		pair.second->update(dt);
	});
}


void World::checkCollision(GameObject* ob1, GameObject* ob2) {

}

