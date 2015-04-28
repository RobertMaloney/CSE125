#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
//#include "Location.h"
//#include <deque>
//#include <unordered_map>
#include <glm.hpp>
//#include <gtc\matrix_transform.hpp>
//#include <gtc\type_ptr.hpp>
#include <gtc\type_ptr.hpp>
//#include <gtx\string_cast.hpp>
#include "Serializable.h"
#include "../graphics/GraphicsEngine.h"


//using std::pair;
//using std::deque;
//using std::make_pair;
//using std::unordered_map;

using glm::mat4;
using glm::vec4;
using namespace std;

typedef unsigned int ObjectId;

class GameObject : public Serializable  {
	//TODO put it outside
protected:

	ObjectId id;
	//Location loc;
	static ObjectId numOfObjects;


	//mat4 location;
	vec4 loc;




public:

	MatrixNode* node;//

	GameObject() :GameObject(505, 0, 0, 0) {};
	GameObject(float radius, float theta, float azimuth, float direction);
	GameObject(const vec4 & loc) :GameObject(loc.r, loc.g, loc.b, loc.a) {};

	virtual ~GameObject();

	void serialize(Packet & p);
	void deserialize(Packet & p);

	ObjectId getId();

	//int getX();
	//int getY();
	//int getZ();

	//Location getLoc();
	vec4 & getLoc();

	//void setX(int newX);
	//void setY(int newY);
	//void setZ(int newZ);

	void setLoc(vec4 & newLoc);

};

#endif