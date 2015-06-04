#include "PhysicsEngine.h"


PhysicsEngine::PhysicsEngine() {
	objectDb = &ObjectDB::getInstance();
	forces.insert(make_pair(GRAVITY, new GravityGenerator()));
	forces.insert(make_pair(DRAG, new DragGenerator()));
}


PhysicsEngine::~PhysicsEngine() {
	for (auto it = forces.begin(); it != forces.end(); ++it) {
		if (it->second) {
			delete it->second;
		}
	}
}


void PhysicsEngine::update(float dt) {
	changed.clear();
	this->generateForces(dt);
	this->integrateObjects(dt);
	this->generateCollisions();
	this->resolveCollisions(dt);
}


vector<GameObject*> & PhysicsEngine::getChangedObjects(){
	return this->changed;
}


void PhysicsEngine::registerInteraction(MoveableObject* object, unsigned int flags) {
	assert(object);

	// check to see if this object is being watched by the engine. if not add it to list of moveable objects
	auto contains = find_if(interactions.begin(), interactions.end(), 
		[&] (pair<MoveableObject*, unsigned int> pair) -> bool { 
		   return pair.first == object;
	});

	if (contains == interactions.end()) {
		interactions.insert(make_pair(object, flags));
		return;
	}
	
	interactions[object] |= flags;
}


void PhysicsEngine::generateCollisions() {
	// loop over each object that has interactions
	for (auto it = interactions.begin(); it != interactions.end(); ++it) {
      if (it->first->getType() == NPCOBJ) continue; // NPCS do not collide

		// loop over all objects
		for (auto jt = objectDb->objects.begin(); jt != objectDb->objects.end(); ++jt) {
		
			// if object is not itself and it is colliding with something then make a collision
			// pair and add it to the set of collisions in this frame.
			if (jt->second != it->first && this->checkCollision(it->first, jt->second)) {

				// this provides ordering on the pairs so we dont get duplicates
				GameObject* l = (it->first < jt->second) ? it->first : jt->second;
				GameObject* r = (l == it->first) ? jt->second : it->first;
				collisionSet.insert(make_pair(l, r));

				// if we hit a static object then we need to make sure we send it in the network update
				if (jt->second->getType() != MOVEABLE) {
					changed.push_back(jt->second);
				}
			}
		}
	}
}


// get all the collisions be sure to avoid duplicate collisions
void PhysicsEngine::resolveCollisions(float dt) {
	for (auto it = collisionSet.begin(); it != collisionSet.end(); ++it) {
		it->first->collide(dt, *it->second);
		it->second->collide(dt, *it->first);
	}
	collisionSet.clear();
}


void PhysicsEngine::generateForces(float dt) {
	for (auto it = interactions.begin(); it != interactions.end(); ++it) {
		if (it->second & GRAVITY) {
			auto forceIterator = forces.find(GRAVITY);
			forceIterator->second->updateForce(it->first, dt);
		}
		if (it->second & DRAG) {
			auto forceIterator = forces.find(DRAG);
			forceIterator->second->updateForce(it->first, dt);
		}
	}
}


// update all the objects in the game by one timestep
void PhysicsEngine::integrateObjects(float dt) {
	for (auto it = interactions.begin(); it != interactions.end(); ++it) {
		it->first->integrate(dt);
		changed.push_back(it->first);
	}
}


void PhysicsEngine::loadConfiguration(Json::Value config) {
	Json::Value & physics = config["physics engine"];
	GravityGenerator* gGenerator = (GravityGenerator*) forces.find(GRAVITY)->second;
	gGenerator->gravity = -1.f * physics["gravity"].asFloat();

	DragGenerator* dGenerator = (DragGenerator*) forces.find(DRAG)->second;
	dGenerator->k1 = physics["k1"].asFloat();
	dGenerator->k2 = physics["k2"].asFloat();
}

