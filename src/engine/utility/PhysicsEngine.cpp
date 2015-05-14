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

// get all the collisions be sure to avoid duplicate collisions
void PhysicsEngine::resolveCollisions(float dt) {

	// lLoop over all pairs of objects. check if they are colliding, if they are call there collision 
	// methods
	for (auto it = moveableObjects.begin(); it != moveableObjects.end(); ++it) {
		for (auto jt = objectDb->objects.begin(); jt != objectDb->objects.end(); ++jt) {
			if (jt->second != *it && this->checkCollision(*it, jt->second)) {
				(*it)->collide(dt, *jt->second);
			}
		}
	}
}


void PhysicsEngine::registerMoveable(MoveableObject* object) {
	assert(object);
	this->moveableObjects.push_back(object);
}


void PhysicsEngine::removeMoveable(MoveableObject* object) {
	assert(object);
	remove_if(this->moveableObjects.begin(), this->moveableObjects.end(), [&] (MoveableObject* other) -> bool { return other == object; });
}


// update all the objects in the game by one timestep
void PhysicsEngine::updateObjects(float dt) {
	for (auto it = moveableObjects.begin(); it != moveableObjects.end(); ++it){
		(*it)->move(dt);
	}
}




