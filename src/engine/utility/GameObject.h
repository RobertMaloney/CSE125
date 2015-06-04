#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <iostream>
#include <algorithm>
#include <glm.hpp>
#include <gtc\quaternion.hpp>
#include "../network/Packet.h"
#include "Serializable.h"
#include "IdGenerator.h"
#include "../utility/Configurable.h"
#include "../physics/Collidable.h"
#include "Model.h"

using glm::mat4;
using glm::vec2;
using glm::vec4;
using glm::quat;
using std::transform;
using namespace std;

enum ObjectType {
	PLAYER,
	MOVEABLE,
	GAMEOBJECT,
	IEATABLE,
    POWERUP
};

class GameObject : public Serializable, public Collidable, public Configurable {

protected:

	quat orientation;
	float angle;
	float height;
	float scale;

	ObjectId id;
	ObjectType type;

	float modelRadius;
	float modelHeight;

	ObjectType typeFromString(string typeName);

	bool visible;

	// Model
	Model rm = TREE;

	//actions
	bool eat;
	bool hit;



public:

	GameObject() :GameObject(505, 0, 0, 0) {};
	GameObject(float radius, float theta, float azimuth, float direction);//TODO

	virtual ~GameObject();

	ObjectId getId();
	void setId(ObjectId theId);

	Model getModel();
	void setModel(Model model);

	quat & getOrientation();
	void moveAngle(float);
	float getAngle();
	float getHeight();
	void setHeight(float h);
	float getScale();
	void setScale(float);

	void setModelHeight(float mheight);
	float getModelHeight();

	bool getVisible();
	void setVisible(bool v);

	bool getEat();
	bool getHit();

	float getModelRadius();
	void setModelRadius(float radius);

	void setDeleteFlag(bool flag);
	bool getDeleteFlag();

	string typeAsString();

	ObjectType getType() const;

	virtual void collide(float dt, GameObject & target);
	virtual void loadConfiguration(Json::Value config);

	void serialize(Packet & p);
	void deserialize(Packet & p);

};

#endif