#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glm.hpp>
#include <gtc\quaternion.hpp>
#include "../network/Packet.h"
#include "Serializable.h"
#include "IdGenerator.h"
#include "Model.h"

using glm::mat4;
using glm::vec2;
using glm::vec4;
using glm::quat;
using namespace std;

enum ObjectType {
	PLAYER,
	MOVEABLE,
	GAMEOBJECT,
    IEATABLE
};

class GameObject : public Serializable  {

protected:

	quat orientation;
	float angle;
	float height;

	ObjectId id;
	ObjectType type;
	float modelRadius;

	bool visible;

    // Model
    Model rm = TREE;


public:

	GameObject(float radius = 505, float theta = 0, float azimuth = 0, float direction = 0);
	GameObject(const vec4 & loc) : GameObject(loc.r, loc.g, loc.b, loc.a) {};

	virtual ~GameObject();

	ObjectId getId();
	void setId(ObjectId theId);

    Model getModel();
    void setModel(Model model);

    quat & getOrientation();
    float getAngle();
    float getHeight();

    bool getVisible();
    void setVisible(bool v);

    float getModelRadius();
    void setModelRadius(float radius);

    void setDeleteFlag(bool flag);
    bool getDeleteFlag();

	ObjectType getType() const;

	void serialize(Packet & p);
	void deserialize(Packet & p);

};

#endif