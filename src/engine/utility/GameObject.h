#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <glm.hpp>
#include <gtc\type_ptr.hpp>
#include "Serializable.h"
#include "IdGenerator.h"

using glm::mat4;
using glm::vec4;
using namespace std;

class GameObject : public Serializable  {

protected:

	ObjectId id;
	vec4 loc;

public:

	GameObject() :GameObject(505, 0, 0, 0) {};
	GameObject(float radius, float theta, float azimuth, float direction);
	GameObject(const vec4 & loc) :GameObject(loc.r, loc.g, loc.b, loc.a) {};

	virtual ~GameObject();

	void serialize(Packet & p);
	void deserialize(Packet & p);

	ObjectId getId();
	void setId(ObjectId theId);

	vec4 & getLoc();
	void setLoc(vec4 & newLoc);

};

#endif