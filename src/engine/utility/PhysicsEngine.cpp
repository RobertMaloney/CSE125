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
	for (auto it = objectDb->objects.begin(); it != objectDb->objects.end(); ++it) {
		for (auto jt = objectDb->objects.begin(); jt != objectDb->objects.end(); ++jt) {
			if (jt != it && this->checkCollision(it->second, jt->second)) {
				it->second->collide(dt, *jt->second);
				jt->second->collide(dt, *it->second);
			}
		}
	}
}


// update all the objects in the game by one timestep
void PhysicsEngine::updateObjects(float dt) {
	for (auto it = objectDb->objects.begin(); it != objectDb->objects.end(); ++it) {
		it->second->update(dt);
	}
}




