#include "Player.h"
#include "Location.h"

int numOfPlayers = 0;

//TODO Config file

Player::Player(BlobModel thebm) {
	numOfPlayers++;
	this->pid = numOfPlayers;
	this->id = "Player" + this->pid;
	this->speed = 0;
	this->power = 0;
	this->loc = Location(0, 0, 0);

	this->bm = thebm;

}

int Player::getSpeed() {
	return speed;
}

int Player::getPower() {
	return power;
}

int Player::getPid() {
	return pid;
}

string Player::getId() {
	return id;
}

int Player::getX() {
	return loc.getX();
}

int Player::getY() {
	return loc.getY();
}

int Player::getZ() {
	return loc.getZ();
}

Location Player::getLoc() {
	return loc;
}

void Player::setLoc(Location newLoc){
	loc = newLoc;
}


void Player::setSpeed(int newSpeed) {
	speed = newSpeed;
}

void Player::setPower(int newPower) {
	power = newPower;
}



void Player::setX(int newX) {
	loc.setX(newX);
}

void Player::setY(int newY) {
	loc.setY(newY);
}

void Player::setZ(int newZ) {
	loc.setZ(newZ);
}