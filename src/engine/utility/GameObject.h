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
//#include "../graphics/GraphicsEngine.h"
#include "IdGenerator.h"
//#include "..\graphics\MatrixNode.h"
#include "ResourceModel.h"


//using std::pair;
//using std::deque;
//using std::make_pair;
//using std::unordered_map;

using glm::mat4;
using glm::vec4;
using namespace std;

class GameObject : public Serializable  {
	//TODO put it outside
protected:

	ObjectId id;
	//Location loc;


	//mat4 location;
	vec4 loc;

   // Model
   ResourceModel rm;


public:

	//MatrixNode* node;//

	GameObject() :GameObject(505, 0, 0, 0) {};
	GameObject(float radius, float theta, float azimuth, float direction);
	GameObject(const vec4 & loc) :GameObject(loc.r, loc.g, loc.b, loc.a) {};

	~GameObject();//virtual?

	void serialize(Packet & p);
	void deserialize(Packet & p);

	ObjectId getId();
	void setId(ObjectId theId);

	//int getX();
	//int getY();
	//int getZ();

	//Location getLoc();
	vec4 & getLoc();
   ResourceModel getModel();

	//void setX(int newX);
	//void setY(int newY);
	//void setZ(int newZ);

	void setLoc(vec4 & newLoc);
   void setModel(ResourceModel model);

};

#endif