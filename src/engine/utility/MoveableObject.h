#ifndef MOVEABLEOBJECT_H
#define MOVEABLEOBJECT_H

#include <glm.hpp>

#include "GameObject.h"

using glm::vec3;

class MoveableObject : public GameObject {

public:

	//MoveableObject(float acceleration = .0005, float dampening = .005);
	MoveableObject(vec3 velocity = vec3(0,0,0), vec3 acceleration = vec3(0,0,0), float dampening = .001);

	virtual ~MoveableObject();

	virtual void move(float dt);
	virtual void collide(float dt, GameObject & target);

protected:

	float dampening;
	float forceAccum;
	vec3 velocity;
	vec3 acceleration;

//	float velocity;
	//float acceleration = .0005;
	//float dampening = .005;

};


#endif