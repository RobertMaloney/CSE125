#ifndef FORCE_GENERATOR_H
#define FORCE_GENERATOR_H

#include "MoveableObject.h"

class ForceGenerator {

public:

	virtual ~ForceGenerator() {}
	virtual void updateForce(MoveableObject* target, float dt) = 0;

};

#endif