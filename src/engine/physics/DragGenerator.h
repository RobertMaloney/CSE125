#ifndef FRICTION_GENERATOR_H
#define FRICTION_GENERATOR_H

#include "ForceGenerator.h"

class DragGenerator : public ForceGenerator {

public:

	DragGenerator();
	virtual ~DragGenerator();

	virtual void updateForce(MoveableObject* target, float dt) override;

	float k1;
	float k2;
	float scale;

};

#endif