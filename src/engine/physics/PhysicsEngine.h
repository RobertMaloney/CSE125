#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H


#include "../utility/ObjectDB.h"
#include "ForceGenerator.h"
#include "GravityGenerator.h"
#include "DragGenerator.h"
#include "MoveableObject.h"

#include <algorithm>
#include <vector>
#include <glm.hpp>
#include <gtc\type_ptr.hpp>
#include <gtx\quaternion.hpp>

using std::make_pair;
using glm::pow;
using glm::sqrt;
using std::vector;
using std::remove_if;

enum {
	GRAVITY = 1 << 0,
	DRAG = 1 << 1,
};


typedef struct {

	ForceGenerator* generator;
	MoveableObject* receiver;

}Interaction;


class PhysicsEngine {

public:
	
	PhysicsEngine();
	~PhysicsEngine();

	void update(float dt);
	vector<GameObject*> & getChangedObjects();

	void registerInteraction(MoveableObject* object, unsigned int flags);
	

private:

	void addInteraction(MoveableObject* object, ForceGenerator* force);

	void generateForces(float dt);
	void integrateObjects(float dt);
	void resolveCollisions(float dt);
	inline bool checkCollision(float dt, MoveableObject* ob1, GameObject* ob2);

	ObjectDB* objectDb;
	vector<GameObject*> changed;
	vector<Interaction> interactions;
	unordered_map<unsigned int, ForceGenerator*> forces;

};


 // check for a collision
 inline bool PhysicsEngine::checkCollision(float dt, MoveableObject* ob1, GameObject* ob2) {
	 // if something is null bad things are happening so crash
	 assert(ob1 && ob2);

	 // get positions in xyz
	 vec3 loc1 = ob1->getOrientation() * glm::vec3(0, 0, 505.f);
	 vec3 loc2 = ob2->getOrientation() * glm::vec3(0, 0, 505.f);

	
	 // if the distance is less than sum of radii there is a collision
	 return glm::distance(loc1, loc2) < (ob1->getModelRadius() + ob2->getModelRadius());
 }

#endif