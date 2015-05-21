#ifndef OBJECTDB_H
#define OBJECTDB_H

#include <iostream>

//#include "IdGenerator.h"
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <glm.hpp>

#include "GameObject.h"


using glm::mat4;
using glm::vec3;
using glm::vec4;
using std::pair;
using std::for_each;
using std::deque;
using std::make_pair;
using std::unordered_map;
using std::cout;


class ObjectDB {

public:

	ObjectDB();
	~ObjectDB();


	bool add(ObjectId theId, GameObject* object);

	bool remove(ObjectId objectId);
	GameObject* get(ObjectId objectId);

	int getSize();
	void getObjectState(vector<Packet> & state);

	static ObjectDB & getInstance();

private:

	unordered_map<ObjectId, GameObject*> objects;

	friend class PhysicsEngine;
   friend class GameState; //lol
};


#endif