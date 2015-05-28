#ifndef MOVEABLEOBJECT_H
#define MOVEABLEOBJECT_H

#include <glm.hpp>
#include <iostream>
#include "../utility/GameObject.h"


using glm::vec3;

typedef struct VerticalMovement {
	float height;
	float velocity;
	float force;
}VerticalMovement;


class MoveableObject : public GameObject {

public:

	//MoveableObject(float acceleration = .0005, float damping = .005);
	MoveableObject() : MoveableObject(505,0,0,0){};
	MoveableObject(float radius, float theta, float azimuth, float direction);
	virtual ~MoveableObject();

	void setMass(float value);
	float getMass();
	float getInverseMass();

	void addForce(vec3 force);
	void addForce(float x, float y, float z);

	void addVelocity(vec3 dv);
	vec3 getVelocity();

	void addAcceleration(vec3 da);
	vec3 getAcceleration();

	void setRestitution(float rest);
	float getRestitution();

	VerticalMovement & getVerticalComponent();

	vec3 rotateInXYPlane(vec3 original, float radians);

	virtual void integrate(float dt);
	virtual void collide(float dt, GameObject & target) override;

protected:


	float inverseMass;
	float restitution;
	vec3 forceAccum;
	vec3 velocity;
	vec3 acceleration;

	VerticalMovement verticalComponent;

};


#endif