#ifndef COLLIDABLE_H
#define COLLIDABLE_H

class GameObject;

class Collidable {

public:

	virtual void collide(float dt, GameObject & target) = 0;

};


#endif