#ifndef GRAVITY_GENERATOR_H
#define GRAVITY_GENERATOR_h

#include "ForceGenerator.h"

class GravityGenerator : public ForceGenerator {

public:

	GravityGenerator();
	virtual ~GravityGenerator();

	virtual void updateForce(MoveableObject* target, float dt) override;

	vec3 gravityForce;

};
#endif