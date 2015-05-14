#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glm.hpp>
#include "../network/Packet.h"
#include "Serializable.h"
#include "IdGenerator.h"
#include "Model.h"

using glm::mat4;
using glm::vec2;
using glm::vec4;
using namespace std;

enum ObjectType {
	PLAYER,
	MOVEABLE,
	GAMEOBJECT
};

class GameObject : public Serializable  {

protected:

	vec4 loc;
	ObjectId id;
	ObjectType type;
	float modelRadius;

   // Model
   Model rm = TREE;


public:

	GameObject(float radius = 505, float theta = 0, float azimuth = 0, float direction = 0);
	GameObject(const vec4 & loc) : GameObject(loc.r, loc.g, loc.b, loc.a) {};

	virtual ~GameObject();

	ObjectId getId();
	void setId(ObjectId theId);

	vec4 & getLoc();
   Model getModel();
	const vec4 & getLocation();
	void setLoc(vec4 & newLoc);
   void setModel(Model model);

	float getModelRadius();
	void setModelRadius(float radius);

	ObjectType getType() const;

	void serialize(Packet & p);
	void deserialize(Packet & p);

};

#endif