#include "ObjectDB.h"



ObjectDB::ObjectDB() {
}


ObjectDB::~ObjectDB() {
	for (auto it = objects.begin(); it != objects.end(); ++it) {
		if (it->second) {
			delete it->second;
		}
	}
}


ObjectId ObjectDB::add(GameObject* object) {
	objects.insert(make_pair(object->getId(), object));
	return object->getId();
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


void ObjectDB::getObjectState(deque<Packet> & state) {
	Packet p;
	for (auto it = objects.begin(); it != objects.end(); ++it){
		it->second->serialize(p);
		state.push_back(p);
		p.clear();
	}
}


ObjectDB & ObjectDB::getInstance() {
	static ObjectDB db;
	return db;
}
