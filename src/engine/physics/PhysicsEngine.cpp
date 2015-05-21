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
	this->resolveCollisions(dt);
}


vector<GameObject*> & PhysicsEngine::getChangedObjects(){
	return this->changed;
}


void PhysicsEngine::registerInteraction(MoveableObject* object, unsigned int flags) {
	assert(object);
	Interaction i;
	i.receiver = object;
	
	// check to see if this object is being watched by the engine. if not add it to list of moveable objects
	auto contains = find_if(moveables.begin(), moveables.end(), 
		[&] (MoveableObject* obj) -> bool { 
			return obj == object; 
	});

	if (contains == moveables.end()) {
		moveables.push_back(object); 
	}

	if (flags & GRAVITY) {
		i.generator = forces.find(GRAVITY)->second;
		interactions.push_back(i);
	}
	if (flags & DRAG) {
		i.generator = forces.find(DRAG)->second;
		interactions.push_back(i);
	}
}


// get all the collisions be sure to avoid duplicate collisions
void PhysicsEngine::resolveCollisions(float dt) {

	// lLoop over all pairs of objects. check if they are colliding, if they are call there collision 
	// methods
	//set<pair<MoveableObject*, GameObject*>> collisions;
	for (auto it = moveables.begin(); it != moveables.end(); ++it) {
		for (auto jt = objectDb->objects.begin(); jt != objectDb->objects.end(); ++jt) {
			
			if (jt->second != *it && this->checkCollision(*it, jt->second)) {
				(*it)->collide(dt, *jt->second);
				if (jt->second->getType() != MOVEABLE) {
					changed.push_back(jt->second);
				}
			}
		}
	}
}


void PhysicsEngine::generateForces(float dt) {
	for (Interaction i : interactions) {
		i.generator->updateForce(i.receiver, dt);
	}
}


// update all the objects in the game by one timestep
void PhysicsEngine::integrateObjects(float dt) {
	for (Interaction i : interactions){
		i.receiver->integrate(dt);
		changed.push_back(i.receiver);
	}
}




