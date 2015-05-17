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


bool ObjectDB::add(ObjectId id, GameObject* object) {
	if (!object) {
		return false;
	}

	auto found = objects.find(id);
	if (found == objects.end()) {
		object->setId(id);
		objects.insert(make_pair(id, object));
		return true;
	}
	return false;
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


void ObjectDB::getObjectState(vector<Packet> & state) {
	Packet p;
	for (auto it = objects.begin(); it != objects.end(); ++it){
		it->second->serialize(p);
		state.push_back(p);
		p.clear();
	}
}

int ObjectDB::getSize(){
	return objects.size();
}

ObjectDB & ObjectDB::getInstance() {
	static ObjectDB db;
	return db;
}
