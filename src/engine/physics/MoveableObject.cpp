#include "MoveableObject.h"

MoveableObject::MoveableObject(float radius, float theta, float azimuth, float direction) : GameObject(radius, theta, azimuth, direction){
	this->velocity = vec3(.00001f, .00001f, 0.f);
	this->setMass(10.f);
	this->restitution = .99f;
	memset((void*) &this->verticalComponent, 0, sizeof(VerticalMovement));
	this->verticalComponent.height = 500.f;
	this->setMassScale(1.f);
	coefficientFriction = .01f;
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

float MoveableObject::getMassScale() {
	return this->massScale;
}

void MoveableObject::setMassScale(float s) {
	this->massScale = s;
}

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


VerticalMovement & MoveableObject::getVerticalComponent() {
	return this->verticalComponent;
	
}


void MoveableObject::setJumpForce(float force) {
	this->jumpForce = force;
}


float MoveableObject::getJumpForce() {
	return this->jumpForce;
}


void MoveableObject::setMoveForce(float force) {
	this->moveForce = force;
}


float MoveableObject::getMoveForce() {
	return this->moveForce;
}

void MoveableObject::setAngleSpeed(float speed) {
	this->angleSpeed = speed;
}

float MoveableObject::getAngleSpeed() {
	return this->angleSpeed;
}

vec3 MoveableObject::rotateInXYPlane(vec3 original, float radians) {
	//std::cout << "rotating velocity : " << radians << std::endl;
	original.x = original.x * glm::cos(glm::radians(radians)) - original.y * glm::sin(glm::radians(radians));
	original.y = original.x * glm::sin(glm::radians(radians)) + original.y * glm::cos(glm::radians(radians));
	return original;
}


float MoveableObject::getFrictionCoefficient() {
	return this->coefficientFriction;
}


void MoveableObject::setFrictionCoefficient(float coeff) {
	this->coefficientFriction = coeff;
}


#include "../utility/Player.h"
void MoveableObject::integrate(float dt) {
	vec3 newAcceleration(0.f,0.f,0.f);
	// use the x-y components to move along the sphere
	vec3 direction = glm::normalize(velocity);
	float magnitude = glm::radians(glm::length(this->velocity));

	// update height. handled by gravity generator
	this->height = verticalComponent.height;

	// do movement along the sphere (position update)
	glm::quat dq = this->orientation * glm::angleAxis(magnitude, direction);
	this->orientation = glm::normalize(glm::mix(this->orientation, dq, dt));

	// calculate the new acceleration based on forces applied in the previous frame
	newAcceleration = this->forceAccum * this->inverseMass + this->acceleration;
	this->forceAccum *= 0.f;
	this->velocity += newAcceleration * dt;

	// temporary form of friction
	/*if (glm::length(this->velocity) > .0001f) {
		if (this->type == PLAYER && dynamic_cast<Player*>(this)->getJumping()) {
			return;
		}
		this->velocity *= .985f;
	}*/

	this->eat = false;
	this->hit = false;
}


void MoveableObject::collide(float dt, GameObject & target) {
	this->velocity *= -1;
}


void MoveableObject::loadConfiguration(Json::Value config) {
	std::cout << " moveableConfig ";
	/*
	    "orientation": null,
        "angle": 0,
        "height": 505,
        "type": "MoveableObject",
        "modelRadius": 1,
        "modelHeight": 1,
        "visible":  true,
		"eat": false,
		"hit": false,
		"scale": 1,
		"model": tree,
        "angleSpeed": 1,
        "jumpForce": 80000,
        "moveForce": 1000,
        "mass": 10,
        "restitution": 1
	*/
	this->verticalComponent.height = this->height;
}