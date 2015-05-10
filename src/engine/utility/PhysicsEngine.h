#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include "ObjectDB.h"
#include <glm.hpp>
#include <gtc\type_ptr.hpp>
#include <gtx\quaternion.hpp>


using std::hash;
using std::pair;

using glm::pow;
using glm::sqrt;

class PhysicsEngine {

public:
	
	PhysicsEngine();
	~PhysicsEngine();

	void update(float dt);

private:

	bool checkCollision(GameObject* ob1, GameObject* ob2);
	void resolveCollisions(float dt);
	void updateObjects(float dt);

	ObjectDB* objectDb;
	
};


 inline vec3 sphereToXYZ(const vec4 & spherePos) {
	vec3 xyz(0, 0, spherePos.x);
	xyz = angleAxis(glm::radians(spherePos.y), vec3(0, 1, 0)) * xyz;
	xyz = angleAxis(glm::radians(spherePos.z), vec3(1, 0, 0)) * xyz;
	return xyz;
}


#endif