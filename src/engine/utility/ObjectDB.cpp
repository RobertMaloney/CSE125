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


GameObject* ObjectDB::add(ObjectId theId, GameObject* object) {


	if (!object) {
		std::cout << "object is null?: " << object << std::endl;
		return NULL;
	}
	if (object->getId()){// should not have id?
		//throw exception TODO
	}

	auto found = objects.find(theId);
	if (found == objects.end()) {
		std::cout << "object not found!. with id: " << theId << " and object: " << object << std::endl;
		object->setId(theId);
		objects.insert(make_pair(theId, object));

		return object;
	}
	return NULL;
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

int ObjectDB::getSize(){
	return objects.size();
}

ObjectDB & ObjectDB::getInstance() {
	static ObjectDB db;
	return db;
}
