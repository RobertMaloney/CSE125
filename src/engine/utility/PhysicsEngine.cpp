#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() {
	objectDb = &ObjectDB::getInstance();
}


PhysicsEngine::~PhysicsEngine() {

}


void PhysicsEngine::update(float dt) {
	this->updateObjects(dt);
	this->resolveCollisions(dt);
}


vector<GameObject*> & PhysicsEngine::getChangedObjects(){
	return this->changed;
}


void PhysicsEngine::registerMoveable(MoveableObject* object) {
	assert(object);
	moveables.push_back(object);
}


void PhysicsEngine::removeMoveable(MoveableObject* object) {
	assert(object);
	remove_if(moveables.begin(), moveables.end(), [&] (MoveableObject* obj) -> bool {return obj == object; });
}

bool contains(vector<MoveableObject*>& container, MoveableObject* object) {
	return find(container.begin(), container.end(), object) != container.end();
}

// get all the collisions be sure to avoid duplicate collisions
void PhysicsEngine::resolveCollisions(float dt) {

	// lLoop over all pairs of objects. check if they are colliding, if they are call there collision 
	// methods
	vector<MoveableObject*> seen;
	for (auto it = moveables.begin(); it != moveables.end(); ++it) {
		for (auto jt = objectDb->objects.begin(); jt != objectDb->objects.end(); ++jt) {
			if (jt->second != *it && /*!contains(seen, *it) &&*/ this->checkCollision(*it, jt->second)) {
				(*it)->collide(dt, *jt->second);
				changed.push_back(jt->second);
			//	seen.push_back(*it);
			}
		}
	}
}




// update all the objects in the game by one timestep
void PhysicsEngine::updateObjects(float dt) {
	for (MoveableObject* object : moveables){
		object->move(dt);
		changed.push_back(object);
	}
}




