#ifndef PHYSICSENGINE_H
#define PHYSICSENGINE_H

#include <set>
#include <iostream>
#include <unordered_map>

#include "ObjectDB.h"

using std::hash;
using std::pair;
using std::set;
using std::unordered_map;

using glm::pow;
using glm::sqrt;

class Collision {

public:

	Collision(GameObject* first, GameObject* second);
	~Collision();

	inline bool operator==(const Collision & rhs)const;
	inline bool operator<(const Collision & rhs)const;
	inline bool operator==(const Collision & rhs);
	inline bool operator<(const Collision & rhs);

	GameObject* first;
	GameObject* second;

};


class PhysicsEngine {

public:
	
	PhysicsEngine();
	~PhysicsEngine();

	void update(float dt);

private:

	bool checkCollision(GameObject* ob1, GameObject* ob2);
	void getCollisions();
	void resolveCollisions();
	void updateObjects(float dt);

	ObjectDB* objectDb;
	set<Collision> collisions;
	
};


 inline vec3 sphereToXYZ(glm::vec4 & spherePos) {
	glm::vec3 xyz(0, 0, spherePos.x);
	xyz = glm::angleAxis(glm::radians(spherePos.y), glm::vec3(0, 1, 0)) * xyz;
	xyz = glm::angleAxis(glm::radians(spherePos.z), glm::vec3(1, 0, 0)) * xyz;
	return xyz;
}


 inline bool Collision::operator==(const Collision & rhs) const {
	 return this->first == rhs.first && this->second == rhs.second;
 }


 inline bool Collision::operator<(const Collision & rhs) const {
	 if (*this == rhs) {
		 return false;
	 }
	 if (this->first == rhs.first) {
		 return this->second < rhs.second;
	 }
	 return this->first < rhs.first;
 }


 inline bool Collision::operator==(const Collision & rhs) {
	 return *this == rhs;
 }


 inline bool Collision::operator<(const Collision & rhs) {
	 return *this < rhs;
 }
#endif