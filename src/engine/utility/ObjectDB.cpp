#include "ObjectDB.h"

void GameObject::serialize(Packet & p) {
	float* loc = glm::value_ptr(location);
	for (int i = 0; i < 16; ++i){
		p.writeFloat(loc[i]);
	}
	p.writeUInt(id);
}


void GameObject::deserialize(Packet & p) {
	float* loc = glm::value_ptr(location);
	for (int i = 0; i < 16; ++i){
		loc[i] = p.readFloat();
	}
	this->id = p.readUInt();
}


ObjectDB::ObjectDB() {
	nextId = 0;
}


ObjectDB::~ObjectDB() {
	for (auto it = objects.begin(); it != objects.end(); ++it) {
		if (it->second) {
			delete it->second;
		}
	}
}


ObjectId ObjectDB::add(GameObject* object) {
	objects.insert(make_pair(nextId, object));
	return nextId++;
}


bool ObjectDB::remove(ObjectId objectId) {
	auto it = objects.find(objectId);
	if (it != objects.end()) {
		delete it->second;
		objects.erase(it);
		return true;
	}
	return false;
}


GameObject* ObjectDB::get(ObjectId objectId) {
	auto it = objects.find(objectId);
	if (it == objects.end()){
		return nullptr;	
	}
	return it->second;
}


deque<Packet> ObjectDB::getObjectState() {
	Packet p;
	deque<Packet> result;
	for (auto it = objects.begin(); it != objects.end(); ++it){
		it->second->serialize(p);
		result.push_back(p);
		p.clear();
	}
	return result;	
}


ObjectDB & ObjectDB::getInstance() {
	static ObjectDB db;
	return db;
}
