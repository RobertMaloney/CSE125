#include "GameObject.h"
//#include "Location.h"
#include "../network/Packet.h"

ObjectId GameObject::numOfObjects = 0;

//TODO Config file
GameObject::GameObject(float radius, float theta, float azimuth, float direction) {
	this->numOfObjects++;
	this->id = numOfObjects;
	//this->loc = Location(nx, ny, nz);
	this->loc = glm::vec4(radius, theta, azimuth, direction);
}

GameObject::~GameObject() {

	delete node;
}


void GameObject::serialize(Packet & p) {
	p.writeUInt(id);
	vec4 refLoc = loc;
	float* loc = glm::value_ptr(refLoc);
	for (int i = 0; i < 16; ++i){
		p.writeFloat(loc[i]);
	}
}


void GameObject::deserialize(Packet & p) {
	//this->id = p.readUInt();
    vec4 refLoc = loc;
	float* loc = glm::value_ptr(refLoc);
	for (int i = 0; i < 16; ++i){
		loc[i] = p.readFloat();
	}
}


ObjectId GameObject::getId() {
	return id;
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