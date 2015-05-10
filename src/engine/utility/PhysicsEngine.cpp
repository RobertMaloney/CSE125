#include "PhysicsEngine.h"


Collision::Collision(GameObject* first, GameObject* second) {
	assert(first && second);
	if (first > second) {
		this->second = first;
		this->first = second;
	} else {
		this->first = first;
		this->second = second;
	}
}


Collision::~Collision() {

}


PhysicsEngine::PhysicsEngine() {
	objectDb = &ObjectDB::getInstance();
}


PhysicsEngine::~PhysicsEngine() {

}


void PhysicsEngine::update(float dt) {
	this->resolveCollisions();
	this->updateObjects(dt);
}

// get all the collisions be sure to avoid duplicate collisions
void PhysicsEngine::getCollisions() {

	// lLoop over all pairs of objects. check if they are colliding, if they are put them in
	// the set of collisions. this should prevent duplicates
	for (auto it = objectDb->objects.begin(); it != objectDb->objects.end(); ++it) {
		for (auto jt = objectDb->objects.begin(); jt != objectDb->objects.end(); ++jt) {
			if (jt != it && this->checkCollision(it->second, jt->second)) {
				collisions.insert(Collision(it->second, jt->second));
			}
		}
	}
}


// dispatch collisions to each object
void PhysicsEngine::resolveCollisions() {
	this->getCollisions();

	for (auto it = collisions.begin(); it != collisions.end(); ++it) {
		it->first->collide(*it->second);
		it->second->collide(*it->first);
	}
	collisions.clear();
}


// update all the objects in the game by one timestep
void PhysicsEngine::updateObjects(float dt) {
	for (auto it = objectDb->objects.begin(); it != objectDb->objects.end(); ++it) {
		it->second->update(dt);
	}
}


// check for a collision
bool PhysicsEngine::checkCollision(GameObject* ob1, GameObject* ob2) {
	// if something is null bad things are happening so crash
	assert(ob1 && ob2);

	// get positions in xyz
	vec3 loc1 = sphereToXYZ(ob1->getLoc());
	vec3 loc2 = sphereToXYZ(ob2->getLoc());

	float r1 = ob1->getModelRadius();
	float r2 = ob2->getModelRadius();

	// calculate distance between positions
	float distance = sqrt(pow(loc1.x - loc2.x, 2) + pow(loc1.y - loc2.y, 2) + pow(loc1.z - loc2.z, 2));

	// if the distance is less than sum of radii there is a collision
	return distance < (r1 + r2);
}

