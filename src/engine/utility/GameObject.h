#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <glm.hpp>
#include <gtc\quaternion.hpp>
#include "../network/Packet.h"
#include "Serializable.h"
#include "IdGenerator.h"
#include "../physics/Collidable.h"
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

class GameObject : public Serializable, public Collidable  {

protected:

	quat orientation;
	float angle;
	float height;

	ObjectId id;
	ObjectType type;

	float modelRadius;
	float modelHeight;

	bool visible;

   // Model
   Model rm = TREE;



public:

	GameObject() :GameObject(505, 0, 0, 0){};
	GameObject(float radius, float theta, float azimuth, float direction);//TODO
	//GameObject(const vec4 & loc) : GameObject(loc.r, loc.g, loc.b, loc.a) {};

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

   void setModelHeight(float mheight);
   float getModelHeight();

   bool getVisible();
   void setVisible(bool v);

   float getModelRadius();
   void setModelRadius(float radius);

   void setDeleteFlag(bool flag);
   bool getDeleteFlag();

	ObjectType getType() const;

	virtual void collide(float dt, GameObject & target);

	void serialize(Packet & p);
	void deserialize(Packet & p);

};

#endif