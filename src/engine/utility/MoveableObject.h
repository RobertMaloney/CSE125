#ifndef MOVEABLE_OBJECT_H
#define MOVEABLE_OBJECT_H

#include "GameObject.h"

class MoveableObject : public GameObject {

public:

	MoveableObject(float acceleration = .0005, float dampening = .005);
	virtual ~MoveableObject();

	virtual void move(float dt);
	virtual void collide(float dt, GameObject & target);

protected:

	float velocity;
	float acceleration = .0005;
	float dampening = .005;

};


#endif