#include "Player.h"


//TODO Config file
Player::Player(Model thebm, float radius, float theta, float azimuth, float direction) : GameObject(radius, theta, azimuth, direction) {
	this->speed = 0;
	this->power = 0;

	this->loc = glm::vec4(radius, theta, azimuth, direction);
	this->rm = thebm;
   this->moves[0] = false;
   this->moves[1] = false;
   this->moves[2] = false;
   this->moves[3] = false;
}

Player::~Player() {

}


int Player::getSpeed() {
	return speed;
}

int Player::getPower() {
	return power;
}


void Player::setSpeed(int newSpeed) {
	speed = newSpeed;
}

void Player::setPower(int newPower) {
	power = newPower;
}

bool Player::getMoving(int index) {
	return moves[index];
}

void Player::setMoving(int index, bool b) {
	moves[index] = b;
}