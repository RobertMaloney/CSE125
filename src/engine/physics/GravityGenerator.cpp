#include "GravityGenerator.h"


GravityGenerator::GravityGenerator() {
	gravityForce = vec3(0.f, 0.f, 0.f);
}


GravityGenerator::~GravityGenerator() {

}


void GravityGenerator::updateForce(MoveableObject* target, float dt) {
	target->addForce(target->getMass() * this->gravityForce);
}