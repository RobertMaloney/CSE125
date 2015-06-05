#include "GameObject.h"


//TODO Config file
GameObject::GameObject(float radius, float theta, float azimuth, float direction) {
	//this->loc = vec4(radius, theta, azimuth, direction);
	this->modelRadius = 1.f;
	this->scale = 1.f;
	this->type = GAMEOBJECT;
	this->angle = direction;
	this->height = radius;
	this->orientation = glm::quat(glm::vec3(theta, azimuth, 0.f));
	this->visible = true;
	this->particle = false;
	this->eat = false;
	this->hit = false;
}

GameObject::~GameObject() {
	// have to remove from tree
	//delete node;
}


void GameObject::serialize(Packet & p) {
	p.writeUInt(id);

	p.writeInt(static_cast<int>(this->type));

	for (int i = 0; i < 4; ++i) {
		//p.writeFloat(this->loc[i]);
		p.writeFloat(this->orientation[i]);
	}
	p.writeFloat(this->angle);
	p.writeFloat(this->height);
	p.writeFloat(this->scale);
	p.writeByte(this->visible);
	p.writeByte(this->particle);
    p.writeInt(static_cast<int>(this->rm));
	p.writeByte(this->eat);
	p.writeByte(this->hit);
}


void GameObject::deserialize(Packet & p) {
	this->id = p.readUInt();

	this->type = static_cast<ObjectType>(p.readInt());
	for (int i = 0; i < 4; ++i) {
		//this->loc[i] = p.readFloat();
		this->orientation[i] = p.readFloat();
	}
	this->angle = p.readFloat();
	this->height = p.readFloat();
	this->scale = p.readFloat();
	this->visible = p.readBool();
	this->particle = p.readBool();
    this->rm = static_cast<Model>(p.readInt());
	this->eat = p.readBool();
	this->hit = p.readBool();
}

quat & GameObject::getOrientation() {
	return orientation;
}

void GameObject::moveAngle(float delta) {
	angle += delta;
}

float GameObject::getAngle() {
	return angle;
}

float GameObject::getHeight() {
	return this->height;
}

void GameObject::setHeight(float height) {
	this->height = height;
}

float GameObject::getScale() {
	return this->scale;
}

void GameObject::setScale(float s) {
	this->scale = s;
}

void GameObject::randScale(float min, float max) {
   float floor = min, ceiling = max, range = (ceiling - floor);
   this->scale = floor + float((range * rand()) / (RAND_MAX + 1.0));
   this->modelHeight = this->modelHeight * this->scale;
   this->modelRadius = this->modelRadius * this->scale;
}

void GameObject::setModelHeight(float mheight) {
	assert(!(mheight < 0.f));
	this->modelHeight = mheight;
}
float GameObject::getModelHeight() {
	return this->modelHeight * this->scale; // scale to maintain collisions;
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

bool GameObject::getVisible(){
	return this->visible;
}

void GameObject::setVisible(bool v){
	this->visible = v;
}

bool GameObject::getParticle(){
	return this->particle;
}

void GameObject::setParticle(bool v){
	this->particle = v;
}

bool GameObject::getEat(){
	return this->eat;
}

bool GameObject::getHit(){
	return this->hit;
}

float GameObject::getModelRadius() {
	return this->modelRadius * this->scale; // scale to maintain collisions
}

void GameObject::setModelRadius(float radius) {
	assert(radius > 0);
	this->modelRadius = radius;
}

ObjectType GameObject::getType() const {
	return this->type;
}


ObjectType GameObject::typeFromString(string typeName) {
	
	transform(typeName.begin(), typeName.end(), typeName.begin(), ::tolower);

	if (typeName == "player") {
		return ObjectType::PLAYER;
	}
	if (typeName == "moveable") {
		return ObjectType::MOVEABLE;
	}
	if (typeName == "ieatable") {
		return ObjectType::IEATABLE;
	}

	return ObjectType::GAMEOBJECT;
}

string GameObject::typeAsString() {
	switch (this->type) {
	case PLAYER:
		return "player";
	case MOVEABLE:
		return "moveable";
	case IEATABLE:
		return "ieatable";
	default:
		return "gameobject";
	}
}

void GameObject::collide(float dt, GameObject & target) {

}


void GameObject::loadConfiguration(Json::Value config) {
	
	//"orientation": null,
	Json::Value & obj = config["game object"];
	this->angle = obj["angle"].asFloat();
	this->height = obj["height"].asFloat();
	this->type = this->typeFromString(obj["type"].asString());
	this->scale = obj["scale"].asFloat();
	this->modelRadius = obj["modelRadius"].asFloat();
	this->modelHeight = obj["modelHeight"].asFloat();
	this->visible = obj["visible"].asBool();
	this->particle = obj["particle"].asBool();
	this->rm = ResourceMap::getModelFromString(obj["model"].asString());
	this->eat = obj["eat"].asBool();
	this->hit = obj["hit"].asBool();
}