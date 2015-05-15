#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H


#include "ObjectDB.h"
#include "MoveableObject.h"

#include <algorithm>
#include <vector>
#include <glm.hpp>
#include <gtc\type_ptr.hpp>
#include <gtx\quaternion.hpp>

using glm::pow;
using glm::sqrt;
using std::vector;
using std::remove_if;

class PhysicsEngine {

public:
	
	PhysicsEngine();
	~PhysicsEngine();

	void update(float dt);
	vector<GameObject*> & getChangedObjects();

	void registerMoveable(MoveableObject* object);
	void removeMoveable(MoveableObject* object);

private:

	void updateObjects(float dt);
	void resolveCollisions(float dt);
	inline bool checkCollision(MoveableObject* ob1, GameObject* ob2);

	ObjectDB* objectDb;
	vector<GameObject*> changed;
	vector<MoveableObject*> moveables;

};


 inline vec3 sphereToXYZ(const vec4 & spherePos) {
	vec3 xyz(0, 0, spherePos.x);
	xyz = angleAxis(glm::radians(spherePos.y), vec3(0, 1, 0)) * xyz;
	xyz = angleAxis(glm::radians(spherePos.z), vec3(1, 0, 0)) * xyz;
	return xyz;
}

 // check for a collision
 inline bool PhysicsEngine::checkCollision(MoveableObject* ob1, GameObject* ob2) {
	 // if something is null bad things are happening so crash
	 assert(ob1 && ob2);

	 // get positions in xyz
	 vec3 loc1 = ob1->getOrientation() * glm::vec3(0, 0, 505.f);
	 vec3 loc2 = ob2->getOrientation() * glm::vec3(0, 0, 505.f);

	 // if the distance is less than sum of radii there is a collision
	 return glm::distance(loc1, loc2) < (ob1->getModelRadius() + ob2->getModelRadius());
 }

#endif