#include "GameObject.h"


//TODO Config file
GameObject::GameObject(float radius, float theta, float azimuth, float direction) {
	//this->loc = vec4(radius, theta, azimuth, direction);
	this->modelRadius = 1.f;
	this->type = GAMEOBJECT;
	this->angle = direction;
	this->orientation = glm::quat(glm::vec3(theta, azimuth, 0.f));
}

GameObject::~GameObject() {
	// have to remove from tree
	//delete node;
}


void GameObject::serialize(Packet & p) {
	p.writeUInt(id);
	for (int i = 0; i < 4; ++i) {
		//p.writeFloat(this->loc[i]);
		p.writeFloat(this->orientation[i]);
	}
	p.writeFloat(this->angle);
   p.writeInt(static_cast<int>(this->rm));
}


void GameObject::deserialize(Packet & p) {
	this->id = p.readUInt();
	for (int i = 0; i < 4; ++i) {
		//this->loc[i] = p.readFloat();
		this->orientation[i] = p.readFloat();
	}
	this->angle = p.readFloat();
   this->rm = static_cast<Model>(p.readInt());
}

quat & GameObject::getOrientation() {
	return orientation;
}

float GameObject::getAngle() {
	return angle;
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

Model GameObject::getModel() {
   return rm;
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


void GameObject::collide(float dt, const GameObject & target) {

}


ObjectType GameObject::getType() const {
	return this->type;
}