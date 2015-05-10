#ifndef OBJECTDB_H
#define OBJECTDB_H

//#include "IdGenerator.h"
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtx\string_cast.hpp>
#include "Serializable.h"
#include "GameObject.h"


using glm::mat4;
using glm::vec3;
using glm::vec4;
using std::pair;
using std::for_each;
using std::deque;
using std::make_pair;
using std::unordered_map;


class ObjectDB {

public:

	ObjectDB();
	~ObjectDB();


	bool add(ObjectId theId, GameObject* object);

	bool remove(ObjectId objectId);
	GameObject* get(ObjectId objectId);

	int getSize();
	void getObjectState(deque<Packet> & state);

	static ObjectDB & getInstance();

private:

	unordered_map<ObjectId, GameObject*> objects;

	friend class PhysicsEngine;
};


#endif