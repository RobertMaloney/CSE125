#ifndef MOVEABLEOBJECT_H
#define MOVEABLEOBJECT_H

#include <glm.hpp>

#include "GameObject.h"

using glm::vec3;

class MoveableObject : public GameObject {

public:

	//MoveableObject(float acceleration = .0005, float dampening = .005);
	MoveableObject();
	virtual ~MoveableObject();

	void setMass(float value);
	float getMass();

	void addForce(vec3 force);
	void addForce(float x, float y, float z);

	virtual void move(float dt);
	virtual void collide(float dt, GameObject & target);

protected:

	float inverseMass;
	float dampening;
	vec3 forceAccum;
	vec3 velocity;
	vec3 acceleration;

//	float velocity;
	//float acceleration = .0005;
	//float dampening = .005;

};


#endif