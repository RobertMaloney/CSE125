#include "World.h"


World::World() {
	objectDb = &ObjectDB::getInstance();
}


World::~World() {

}


void World::update(float dt) {
	static int collisionCounter;
	for (auto it = objectDb->objects.begin(); it != objectDb->objects.end(); ++it) {
		it->second->update(dt);
	}

	for (auto it = objectDb->objects.begin(); it != objectDb->objects.end(); ++it) {
		for (auto jt = objectDb->objects.begin(); jt != objectDb->objects.end(); ++jt) {
			if (jt != it && this->checkCollision(it->second, jt->second)) {
				std::cout << "collision : " << collisionCounter << std::endl;
			}
		}
	}
}


bool World::checkCollision(GameObject* ob1, GameObject* ob2) {
	// if something is null bad things are happening so crash
	assert(ob1 && ob2);

	// get positions in xyz
	vec3 loc1 = this->sphereToXYZ(ob1->getLoc());
	vec3 loc2 = this->sphereToXYZ(ob2->getLoc());

	float r1 = ob1->getModelRadius();
	float r2 = ob2->getModelRadius();

	// calculate distance between positions
	float distance = sqrt(pow(loc1.x - loc2.x, 2) + pow(loc1.y - loc2.y, 2) + pow(loc1.z - loc2.z, 2));

	// if the distance is less than sum of radii there is a collision
	return distance < (r1 + r2);
}

