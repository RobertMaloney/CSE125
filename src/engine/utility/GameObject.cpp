#include "GameObject.h"
//#include "Location.h"
#include "../network/Packet.h"



//TODO Config file
GameObject::GameObject(float radius, float theta, float azimuth, float direction) {
	//this->loc = Location(nx, ny, nz);
	this->loc = glm::vec4(radius, theta, azimuth, direction);
}

GameObject::~GameObject() {
	// have to remove from tree
	//delete node;
}


void GameObject::serialize(Packet & p) {
	p.writeUInt(id);
	for (int i = 0; i < 4; ++i){
		p.writeFloat(this->loc[i]);
	}
}


void GameObject::deserialize(Packet & p) {
	//this->id = p.readUInt();
	for (int i = 0; i < 4; ++i){
		this->loc[i] = p.readFloat();
	}
}


ObjectId GameObject::getId() {
	return id;
}

void GameObject::setId(ObjectId theId) {
	this->id = theId;
}

/*int GameObject::getX() {
	return loc.getX();
}

int GameObject::getY() {
	return loc.getY();
}

int GameObject::getZ() {
	return loc.getZ();
}*/

vec4 & GameObject::getLoc() {
	return loc;
}


void GameObject::setLoc(vec4 & newLoc){
	loc = newLoc;
}



/*void GameObject::setX(int newX) {
	loc.setX(newX);
}

void GameObject::setY(int newY) {
	loc.setY(newY);
}

void GameObject::setZ(int newZ) {
	loc.setZ(newZ);
}*/