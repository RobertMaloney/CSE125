#include "Player.h"
#include <iostream>
#include <gtx\string_cast.hpp>

//TODO Config file
Player::Player(Model thebm, float radius, float theta, float azimuth, float direction) : MoveableObject() {
	static int num;
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
	if (num)
		this->setMass(100);
	num++; 
}

Player::~Player() {

}


bool Player::getMoving(int index) {
	return moves[index];
}

void Player::setMoving(int index, bool b) {
	moves[index] = b;
}


void Player::move(float dt) {

	//this->velocity.x = .0005f;
	if (moves[RIGHT]) {
		angle -= 1.f;
	}
	if (moves[LEFT]) {
		angle += 1.f;
		std::cout << "Angle: " << angle << std::endl;
	}

	float cosa = glm::cos(glm::radians(angle));
	float sina = glm::sin(glm::radians(angle));

	float forceConst = .0005f * log(this->getMass());

	if (moves[UP]) {
		this->addForce(cosa * forceConst, sina * forceConst, 0);

	}

	if (moves[DOWN]) {
		this->addForce(cosa * -forceConst, sina * -forceConst, 0);
	//	this->addForce(0.f, 0.f, forceConst);
	}

	MoveableObject::move(dt);

}


void Player::collide(float dt, GameObject & target) {
	MoveableObject::move(-dt);
	if (target.getType() == PLAYER) {
		std::cout << "target is player " << std::endl;
		Player & p2 = dynamic_cast<Player&>(target);
		vec3 v1 = this->velocity;
		vec3 v2 = p2.velocity;
		vec3 l1 = this->orientation * vec3(0.f, 0.f, 505.f);
		vec3 l2 = p2.orientation * vec3(0.f, 0.f, 505.f);
		float m1 = this->getMass();
		float m2 = p2.getMass();

	/*	vec3 normal = glm::normalize(l1 - l2);
		float seperatingVel = glm::dot(v1 - v2, normal);
		if (seperatingVel > 0){
			return;
		}
		float newSeperatingVel = -seperatingVel * .99f;
		float dv = newSeperatingVel - seperatingVel;

		float totalInverseMass = this->inverseMass + p2.inverseMass;

		float impulse = dv / totalInverseMass;
		vec3 imass = normal * impulse;

		this->velocity = this->velocity + (imass * -this->inverseMass);
		p2.velocity = p2.velocity + (imass * -p2.inverseMass);
		//float closingV = glm::dot(v1, glm::normalize(l2 - l1)) + glm::dot(v2, glm::normalize(l1 - l2));
		*/
	/*	float temp;
		vec3 tangent;
		vec3 normal = glm::normalize(l2 - l1);
		tangent.x = -normal.y;
		tangent.y = normal.x;
		//tangent.z = 0.f;
		float v1n = glm::dot(normal, v1);
		float v1t = glm::dot(tangent, v1);
		float v2n = glm::dot(normal, v2);
		float v2t = glm::dot(tangent, v2); 
	*/	this->velocity = (v1 * (m1 - m2) + 2 * m2 * v2) / (m1 + m2);
		/*v2n = (v2n * (m2 - m1) + 2 * m1 * v1n) / (m1 + m2);
		this->velocity = v1n * normal + v1t * tangent;
		p2.velocity = v2n * normal + v2t * tangent;*/
//		this->velocity = (v1 * (m1 - m2) + 2 * m2 * v2) / (m1 + m2);
//		float mag1 = glm::length(l1 - l2);
	//	float mag2 = glm::length(l2 - l1);
		
	//	mag1 *= mag1;
//		mag2 *= mag2;

	//	this->velocity = v1 - ((2 * m2) / (m1 + m2)) * (glm::dot(v1 - v2, l1 - l2) / mag1) * (l1 - l2);
	//	p2.velocity =    v2 - ((2 * m1) / (m1 + m2)) * (glm::dot(v2 - v1, l2 - l1) / mag2) * (l2 - l1);
	}
	//set target flag = false//dead
	//increase score
	//Render needs to figure out (not) rendering dead object
}