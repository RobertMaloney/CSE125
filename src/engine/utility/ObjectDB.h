#ifndef OBJECTDB_H
#define OBJECTDB_H

#include <deque>
#include <unordered_map>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <gtx\string_cast.hpp>
#include "Serializable.h"
#include "../graphics/GraphicsEngine.h"
#include "GameObject.h"

using glm::mat4;
using glm::vec3;
using glm::vec4;
using std::pair;
using std::deque;
using std::make_pair;
using std::unordered_map;

//ObjectId ObjectDB::numOfObjects = 0;

class ObjectDB {

public:
	//static ObjectId numOfObjects;

	ObjectDB();
	~ObjectDB();


	GameObject* add(ObjectId theId, GameObject* object);

	bool remove(ObjectId objectId);
	GameObject* get(ObjectId objectId);

	void getObjectState(deque<Packet> & state);

	static ObjectDB & getInstance();

private:

	unordered_map<ObjectId, GameObject*> objects;
};


#endif