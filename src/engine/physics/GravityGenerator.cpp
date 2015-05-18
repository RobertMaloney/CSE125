#include "GravityGenerator.h"


GravityGenerator::GravityGenerator() {
	gravity = -10.f;
}


GravityGenerator::~GravityGenerator() {

}


void GravityGenerator::updateForce(MoveableObject* target, float dt) {
	//target->addForce(target->getMass() * this->gravity);
}