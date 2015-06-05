#include "DragGenerator.h"

DragGenerator::DragGenerator() {
	k1 = 1.f;
	k2 = 1.f;
	scale = 1.f;
}


DragGenerator::~DragGenerator() {

}


void DragGenerator::updateForce(MoveableObject* target, float dt) {
	VerticalMovement verticalComponent = target->getVerticalComponent();
	vec3 force = target->getVelocity();
	float magnitude = glm::length(force);
	force = glm::normalize(force);

	//std::cout << "k1 : " << k1 << " k2 : " << k2 << " scale : " << this->scale << std::endl;
	
	magnitude = -(k1 * magnitude + k2 * magnitude * magnitude);
	
	if (verticalComponent.height > 505.f) {
		magnitude *= scale;
	}
	
	force *= magnitude;

	target->addForce(force);
}