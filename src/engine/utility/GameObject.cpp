#include "GameObject.h"
#include "Location.h"

//TODO Config file
GameObject::GameObject(int nx, int ny, int nz) {
	this->numOfObjects++;
	this->id = numOfObjects;
	this->loc = Location(nx, ny, nz);
}

ObjectId GameObject::getId() {
	return id;
}

int GameObject::getX() {
	return loc.getX();
}

int GameObject::getY() {
	return loc.getY();
}

int GameObject::getZ() {
	return loc.getZ();
}

Location GameObject::getLoc() {
	return loc;
}

void GameObject::setLoc(Location newLoc){
	loc = newLoc;
}



void GameObject::setX(int newX) {
	loc.setX(newX);
}

void GameObject::setY(int newY) {
	loc.setY(newY);
}

void GameObject::setZ(int newZ) {
	loc.setZ(newZ);
}