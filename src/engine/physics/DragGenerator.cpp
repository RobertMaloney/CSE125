#include "DragGenerator.h"

DragGenerator::DragGenerator() {
	k1 = 1.f;
	k2 = 1.f;
}


DragGenerator::~DragGenerator() {

}


void DragGenerator::updateForce(MoveableObject* target, float dt) {
	vec3 force = target->getVelocity();
	float magnitude = glm::length(force);
	force = glm::normalize(force);

	force *= -(k1 * magnitude + k2 * magnitude * magnitude);

	target->addForce(force);
}