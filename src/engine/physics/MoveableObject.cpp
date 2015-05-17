#include "MoveableObject.h"

/*MoveableObject::MoveableObject(float acceleration, float damping) : GameObject(), velocity(0) {

}*/
MoveableObject::MoveableObject() : GameObject(){
	this->velocity = vec3(.00001f, .00001f, 0.f);
	//this->damping = .98f;
	this->setMass(10.f);
	//this->friction = .62;
	this->restitution = .99f;
}


MoveableObject::~MoveableObject() {

}


void MoveableObject::setMass(float m) {
	assert(m > 0.f);
	this->inverseMass = 1.f / m;
}


float MoveableObject::getMass() {
	return 1.f / this->inverseMass;
}


float MoveableObject::getInverseMass() {
	return this->inverseMass;
}

/*
void MoveableObject::setFriction(float value) {
	this->friction = value;
}


float MoveableObject::getFriction() {
	return this->friction;
}

*/
void MoveableObject::addForce(vec3 force) {
	this->forceAccum += force;
}


void MoveableObject::addForce(float x, float y, float z) {
	this->forceAccum.x += x;
	this->forceAccum.y += y;
	this->forceAccum.z += z;
}


void MoveableObject::addVelocity(vec3 dv) {
	this->velocity += dv;
}


vec3 MoveableObject::getVelocity() {
	return this->velocity;
}


void MoveableObject::addAcceleration(vec3 da) {
	this->acceleration += da;
}


vec3 MoveableObject::getAcceleration() {
	return this->acceleration;
}


void MoveableObject::setRestitution(float rest) {
	assert(rest >= 0.f && rest <= 1.f);
	this->restitution = rest;
}


float MoveableObject::getRestitution() {
	return this->restitution;
}


void MoveableObject::integrate(float dt) {
	vec3 newAcceleration(0.f,0.f,0.f);
	vec3 direction = glm::normalize(velocity);
	float magnitude = glm::radians(glm::length(this->velocity));

	glm::quat dq = this->orientation * glm::angleAxis(magnitude, direction);
	this->orientation = glm::normalize(glm::mix(this->orientation, dq, dt));
//	this->orientation = dq;

	newAcceleration = this->forceAccum * this->inverseMass + this->acceleration;
	this->forceAccum *= 0.f;
	this->velocity += newAcceleration * dt;

	//if (glm::length(this->velocity) > .00001f){
//		this->velocity *= powf(damping, dt);
	//}
	if (glm::length(this->velocity) > .0001f)
		this->velocity *= .995f;
	
}


void MoveableObject::collide(float dt, GameObject & target) {
	this->velocity *= -1;
}