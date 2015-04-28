#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include "Location.h"
//#include <deque>
//#include <unordered_map>
//#include <glm.hpp>
//#include <gtc\matrix_transform.hpp>
//#include <gtc\type_ptr.hpp>
//#include <gtx\string_cast.hpp>
#include "Serializable.h"
#include "../graphics/GraphicsEngine.h"

//using glm::vec3;
//using std::pair;
//using std::deque;
//using std::make_pair;
//using std::unordered_map;

using glm::mat4;
using namespace std;

typedef unsigned int ObjectId;

class GameObject : Serializable  {
	//TODO put it outside
protected:

	ObjectId id;
	Location loc;
	static ObjectId numOfObjects;


	mat4 location;

	MatrixNode* node;//


public:

	GameObject() :GameObject(0, 0, 0) {};
	GameObject(int x, int y, int z);
	GameObject(Location loc);

	virtual ~GameObject();

	void serialize(Packet & p);
	void deserialize(Packet & p);

	ObjectId getId();

	int getX();
	int getY();
	int getZ();

	Location getLoc();

	void setX(int newX);
	void setY(int newY);
	void setZ(int newZ);

	void setLoc(Location newLoc);

};

#endif