#include "GameObject.h"
#include "../network/Packet.h"


//TODO Config file
GameObject::GameObject(float radius, float theta, float azimuth, float direction) {
	this->loc = glm::vec4(radius, theta, azimuth, direction);
	this->modelRadius = 1.f;
	this->type = ObjectType::GAMEOBJECT;
}

GameObject::~GameObject() {
	// have to remove from tree
	//delete node;
}


void GameObject::serialize(Packet & p) {
	p.writeUInt(id);
	for (int i = 0; i < 4; ++i) {
		p.writeFloat(this->loc[i]);
	}
}


void GameObject::deserialize(Packet & p) {
	this->id = p.readUInt();
	for (int i = 0; i < 4; ++i) {
		this->loc[i] = p.readFloat();
	}
}


ObjectId GameObject::getId() {
	return id;
}

void GameObject::setId(ObjectId theId) {
	this->id = theId;
}


vec4 GameObject::getLoc() {
	return loc;
}

const vec4 & GameObject::getLoc() const {
	return loc;
}


void GameObject::setLoc(vec4 & newLoc) {
	loc = newLoc;
}


float GameObject::getModelRadius() {
	return this->modelRadius;
}


void GameObject::setModelRadius(float radius) {
	assert(radius > 0);
	this->modelRadius = radius;
}


void GameObject::update(float dt) {
}