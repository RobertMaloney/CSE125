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
	this->modelRadius = 5.f;
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
	if (moves[UP]) {
		velocity += acceleration;
	}
	if (moves[DOWN]) {
		velocity -= acceleration;
	}
	if (moves[RIGHT]) {
		angle -= 1.f;
	}
	if (moves[LEFT]) {
		angle += 1.f;
		std::cout << "Angle: " << angle << std::endl;
	}
	// if there was no input simulate friction
	if (!moves[UP] && !moves[DOWN]) {
		if (glm::abs(velocity) <= dampening && glm::abs(velocity) >= 0) {
			velocity = 0;
		} else { 
			velocity = (velocity > 0) ? velocity - dampening : velocity + dampening;
		}
	}
	// move the player
	float cosa = glm::cos(glm::radians(angle));
	float sina = glm::sin(glm::radians(angle));
	glm::quat q = orientation * glm::angleAxis(glm::radians(velocity), glm::vec3(cosa, sina, 0));
	orientation = glm::normalize(glm::mix(orientation, q, dt));
}


void Player::collide(float dt, GameObject & target) {
	this->velocity *= -1;
	//set target flag = false//dead
	//increase score
	//Render needs to figure out (not) rendering dead object
}