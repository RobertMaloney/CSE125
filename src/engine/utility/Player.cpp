#include "Player.h"
#include <iostream>
#include <gtx\string_cast.hpp>

//TODO Config file
Player::Player(Model thebm, float radius, float theta, float azimuth, float direction) : MoveableObject() {
	//this->loc = vec4(radius, theta, azimuth, direction);
   this->rm = thebm;
   this->moves[0] = false;
   this->moves[1] = false;
   this->moves[2] = false;
   this->moves[3] = false;

	for (int i = 0; i < 4; ++i)
		this->moves[i] = false;

	this->type = PLAYER;
	//this->velocity = 0;
	this->modelRadius = 7.f;
	this->setMass(10.f);

}

Player::~Player() {

}


bool Player::getMoving(int index) {
	return moves[index];
}

void Player::setMoving(int index, bool b) {
	moves[index] = b;
}



void Player::integrate(float dt) {

	if (moves[RIGHT]) {
		angle -= 1.f;
		this->velocity = this->rotateInXYPlane(this->velocity, glm::radians(-1.f));
	}
	if (moves[LEFT]) {
		angle += 1.f;
		this->velocity = this->rotateInXYPlane(this->velocity, glm::radians(1.f));
	}

	float cosa = glm::cos(glm::radians(angle));
	float sina = glm::sin(glm::radians(angle));
	

	float forceConst = 100.f * log(this->getMass());

	if (moves[UP]) {
		this->addForce(cosa * forceConst, sina * forceConst, 0.f);

	}
	
	if (moves[DOWN]) {
		this->addForce(cosa * -forceConst, sina * -forceConst, 0.f);
	}


	MoveableObject::integrate(dt);

}


void Player::collide(float dt, GameObject & target) {
	if (target.getType() == PLAYER) {
		Player & other = dynamic_cast<Player&>(target);
		vec3 p1 = this->orientation * vec3(0.f, 0.f, 505.f);
		vec3 p2 = other.orientation * vec3(0.f, 0.f, 505.f);

		vec3 v1 = this->velocity;
		vec3 v2 = other.velocity;
		float m1 = this->getMass();
		float m2 = other.getMass();

		float im1 = this->getInverseMass();
		float im2 = other.getInverseMass();

		float totalMass = m1 + m2;
		
		vec3 normal = p1 - p2;
		vec3 unitNormal = glm::normalize(normal);
		unitNormal.z = 0.f;
		float seperatingVelocity = glm::dot(v1 - v2, unitNormal);

		float p1weight = (totalMass - m1) / totalMass;
		float p2weight = (totalMass - m2) / totalMass;

		float deltaVelocity = (-seperatingVelocity * 1.f) - seperatingVelocity;
		vec3 impulse = unitNormal * (deltaVelocity / (im1 + im2));
		
		this->velocity += impulse * im1; 
		other.velocity -= impulse * im2;
		
		
		if (glm::length(normal) < this->getModelRadius() + other.getModelRadius()) {
			float pen = this->getModelRadius() + other.getModelRadius() - glm::length(normal);
			this->orientation *= glm::angleAxis(glm::radians(p1weight*pen), unitNormal/*-glm::normalize(v1)*/);
			other.orientation *= glm::angleAxis(glm::radians(p2weight*pen), unitNormal/*-glm::normalize(v2)*/);
		}
	/*	vec3 p1mp2 = p1 - p2;
		vec3 unormal12 = p1mp2/ (glm::length(p1mp2));
		vec3 unormal21 = -p1mp2 / (glm::length(-p1mp2));

		float component12 = glm::dot(v1, p1mp2) / glm::length(p1mp2);
		float component21 = glm::dot(v2, -p1mp2) / glm::length(-p1mp2);

		vec3 u12 = component12 * unormal12;
		vec3 u21 = component21 * unormal21;
		vec3 t12 = v1 - u12;
		vec3 t21 = v2 - u21;

		vec3 v12 = (u12 * (m1 - m2) + 2 * m2 * u21) / (m1 + m2);
		vec3 v21 = (u21 * (m2 - m1) + 2 * m1 * u12) / (m1 + m2);
		
		this->velocity = v12 + t12;
		other.velocity = v21 + t21;*/
	}

}
/*
vec3 movePerImass = unitNormal * (penetration / (im1 + im2));
this->orientation *= glm::angleAxis(glm::radians(im1 * glm::length(movePerImass)), glm::normalize(this->velocity + movePerImass));
other.orientation *= glm::angleAxis(glm::radians(im2 * -glm::length(movePerImass)), glm::normalize(other.velocity + movePerImass));
*/