#include "Player.h"
//#include "Location.h"

//ObjectId numOfPlayers = 0;

//TODO Config file
Player::Player(BlobModel thebm, float radius, float theta, float azimuth, float direction) : GameObject(radius, theta, azimuth, direction) {
	//this->numOfPlayers++;
	//this->pid = numOfPlayers;
	this->speed = 0;
	this->power = 0;
	//this->loc = Location(nx, ny, nz);
	this->loc = glm::vec4(radius, theta, azimuth, direction);
	this->bm = thebm;

	for (int i = 0; i < 4; ++i)
		this->moves[i] = false;
}

/*glm::vec4 & Player::getOrientation() {
	return orientation;
}*/

int Player::getSpeed() {
	return speed;
}

int Player::getPower() {
	return power;
}

//ObjectId Player::getPid() {
//	return pid;
//}


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