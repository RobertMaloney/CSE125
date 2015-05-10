#include "PhysicsEngine.h"


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
void PhysicsEngine::getCollisions(set<pair<GameObject*, GameObject*>> & collisions) {
	// loop over all pairs for now
	for (auto it = objectDb->objects.begin(); it != objectDb->objects.end(); ++it) {
		for (auto jt = objectDb->objects.begin(); jt != objectDb->objects.end(); ++jt) {
			// check to see if they are colliding
			if (jt != it && this->checkCollision(it->second, jt->second)) {
				// add them to collision set, avoid duplicates
				GameObject* first = it->second;
				GameObject* second = jt->second;
				if (first > second) {
					collisions.insert(make_pair(second, first));
				} else {
					collisions.insert(make_pair(first, second));
				}
			}
		}
	}
}

// dispatch collisions to each object
void PhysicsEngine::resolveCollisions() {
	set<pair<GameObject*, GameObject*>> collisions;
	this->getCollisions(collisions);

	for (auto it = collisions.begin(); it != collisions.end(); ++it) {
		it->first->collide(*it->second);
		it->second->collide(*it->first);
	}
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

