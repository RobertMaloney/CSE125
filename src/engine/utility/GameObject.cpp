#include "GameObject.h"
#include "Location.h"
#include "../network/Packet.h"

//TODO Config file
GameObject::GameObject(int nx, int ny, int nz) {
	this->numOfObjects++;
	this->id = numOfObjects;
	this->loc = Location(nx, ny, nz);
}

void GameObject::serialize(Packet & p) {
	p.writeUInt(id);
	float* loc = glm::value_ptr(location);
	for (int i = 0; i < 16; ++i){
		p.writeFloat(loc[i]);
	}
}


void GameObject::deserialize(Packet & p) {
	//this->id = p.readUInt();
	float* loc = glm::value_ptr(location);
	for (int i = 0; i < 16; ++i){
		loc[i] = p.readFloat();
	}
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