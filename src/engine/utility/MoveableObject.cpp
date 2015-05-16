#include "MoveableObject.h"

/*MoveableObject::MoveableObject(float acceleration, float dampening) : GameObject(), velocity(0) {

}*/
MoveableObject::MoveableObject() : GameObject(){
	this->velocity = vec3(.0001f, .0001f, 0.f);
	this->inverseMass = .2f;
	this->dampening = .97f;
}


MoveableObject::~MoveableObject() {

}


void MoveableObject::setMass(float m) {
	assert(m > 0);
	this->inverseMass = 1.f / m;
}


float MoveableObject::getMass() {
	return 1.f / this->inverseMass;
}


void MoveableObject::addForce(vec3 force) {
	this->forceAccum.x += force.x;
	this->forceAccum.y += force.y;
	this->forceAccum.z += force.z;
}


void MoveableObject::addForce(float x, float y, float z) {
	this->forceAccum.x += x;
	this->forceAccum.y += y;
	this->forceAccum.z += z;
}


void MoveableObject::move(float dt) {
	glm::quat dq;
	vec3 newAcceleration(0,0,0);


	vec3 unit = glm::normalize(velocity);
	dq = this->orientation * glm::angleAxis(glm::radians(glm::length(this->velocity)), unit);
	this->orientation = glm::normalize(glm::mix(this->orientation, dq, dt));

	newAcceleration = this->forceAccum * this->inverseMass + this->acceleration;
	this->forceAccum = vec3(0, 0, 0);
	this->velocity += newAcceleration * dt;
	if (glm::length(this->velocity) > .0001f){
		this->velocity = dampening * this->velocity;
	}
	
}


void MoveableObject::collide(float dt, GameObject & target) {
	this->velocity *= -1;
}