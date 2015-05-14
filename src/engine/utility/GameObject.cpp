#include "GameObject.h"


//TODO Config file
GameObject::GameObject(float radius, float theta, float azimuth, float direction) {
	this->loc = vec4(radius, theta, azimuth, direction);
	this->modelRadius = 1.f;
	this->type = GAMEOBJECT;
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
   p.writeInt(static_cast<int>(this->rm));
}


void GameObject::deserialize(Packet & p) {
	this->id = p.readUInt();
	for (int i = 0; i < 4; ++i) {
		this->loc[i] = p.readFloat();
	}
   this->rm = static_cast<Model>(p.readInt());
}


ObjectId GameObject::getId() {
	return id;
}

void GameObject::setId(ObjectId theId) {
	this->id = theId;
}

void GameObject::setModel(Model model) {
   this->rm = model;
}

vec4 & GameObject::getLoc() {
	return loc;
}

Model GameObject::getModel() {
   return rm;
}

const vec4& GameObject::getLocation() {
	return this->loc;
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


ObjectType GameObject::getType() const {
	return this->type;
}