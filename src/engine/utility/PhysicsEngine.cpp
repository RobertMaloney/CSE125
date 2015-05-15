#include "PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() {
	objectDb = &ObjectDB::getInstance();
}


PhysicsEngine::~PhysicsEngine() {

}


void PhysicsEngine::update(float dt) {
	this->resolveCollisions(dt);
	this->updateObjects(dt);
}


void PhysicsEngine::registerMoveable(MoveableObject* object) {
	assert(object);
	moveables.push_back(object);
}


void PhysicsEngine::removeMoveable(MoveableObject* object) {
	assert(object);
	remove_if(moveables.begin(), moveables.end(), [&] (MoveableObject* obj) -> bool {return obj == object; });
}

// get all the collisions be sure to avoid duplicate collisions
void PhysicsEngine::resolveCollisions(float dt) {

	// lLoop over all pairs of objects. check if they are colliding, if they are call there collision 
	// methods
	for (auto it = moveables.begin(); it != moveables.end(); ++it) {
		for (auto jt = objectDb->objects.begin(); jt != objectDb->objects.end(); ++jt) {
			if (jt->second != *it && this->checkCollision(*it, jt->second)) {
				(*it)->collide(dt, *jt->second);
			}
		}
	}
}


// update all the objects in the game by one timestep
void PhysicsEngine::updateObjects(float dt) {
	for (MoveableObject* object : moveables){
		object->move(dt);
	}
}




