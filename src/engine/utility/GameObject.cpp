#include "GameObject.h"
#include "../network/Packet.h"


//TODO Config file
GameObject::GameObject(float radius, float theta, float azimuth, float direction) {
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
   p.writeInt(static_cast<int>(this->rm));
}


void GameObject::deserialize(Packet & p) {
	this->id = p.readUInt();
	for (int i = 0; i < 4; ++i){
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


vec4 & GameObject::getLoc() {
	return loc;
}


void GameObject::setLoc(vec4 & newLoc){
	loc = newLoc;
}

Model GameObject::getModel() {
   return rm;
}
