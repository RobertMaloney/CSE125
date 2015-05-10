#include "Physics.h"


Physics::Physics() {
	objectDb = &ObjectDB::getInstance();
}


Physics::~Physics() {

}


void Physics::update(float dt) {
	this->handleCollisions();
	this->updateObjects(dt);
}


void Physics::handleCollisions() {
	static int collisionCounter;

	for (auto it = objectDb->objects.begin(); it != objectDb->objects.end(); ++it) {
		for (auto jt = objectDb->objects.begin(); jt != objectDb->objects.end(); ++jt) {
			// check all pairs for now
			if (jt != it && this->checkCollision(it->second, jt->second)) {
				std::cout << "collision : " << collisionCounter++ << std::endl;
			}
		}
	}
}


void Physics::updateObjects(float dt) {
	for (auto it = objectDb->objects.begin(); it != objectDb->objects.end(); ++it) {
		it->second->update(dt);
	}
}


bool Physics::checkCollision(GameObject* ob1, GameObject* ob2) {
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

/*
void GameState::updateMovingPlayers() {
// update positions
for (auto it = players.begin(); it != players.end(); ++it) {
if ((*it)->getMoving(Player::UP)) {
float dir = (*it)->getLoc().w;
(*it)->getLoc().z += glm::cos(glm::radians(dir));
(*it)->getLoc().y += glm::sin(glm::radians(dir));
}
if ((*it)->getMoving(Player::RIGHT)) {
(*it)->getLoc().w -= 1.f;

}
if ((*it)->getMoving(Player::DOWN)) {
float dir = (*it)->getLoc().w;
(*it)->getLoc().z -= glm::cos(glm::radians(dir));
(*it)->getLoc().y -= glm::sin(glm::radians(dir));

}
if ((*it)->getMoving(Player::LEFT)) {
(*it)->getLoc().w += 1.f;

}
}
}*/