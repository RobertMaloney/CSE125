#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <glm.hpp>
#include <gtc\type_ptr.hpp>
#include "Serializable.h"
#include "IdGenerator.h"

using glm::mat4;
using glm::vec2;
using glm::vec4;
using namespace std;

class GameObject : public Serializable  {

public:

	enum ObjectType {
		PLAYER,
		MOVEABLE,
		GAMEOBJECT
	};

protected:

	vec4 loc;
	ObjectId id;
	ObjectType type;
	float modelRadius;

public:

	GameObject(float radius = 505, float theta = 0, float azimuth = 0, float direction = 0);
	GameObject(const vec4 & loc) : GameObject(loc.r, loc.g, loc.b, loc.a) {};

	virtual ~GameObject();

	void serialize(Packet & p);
	void deserialize(Packet & p);

	ObjectId getId();
	void setId(ObjectId theId);

	vec4 getLoc();
	const vec4 & getLoc() const;
	void setLoc(vec4 & newLoc);

	float getModelRadius();
	void setModelRadius(float radius);

	ObjectType getType();

	virtual void update(float dt);

};

#endif