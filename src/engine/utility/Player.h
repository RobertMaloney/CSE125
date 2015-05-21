#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Model.h"
#include "../physics/MoveableObject.h"

using namespace std;

class Player : public MoveableObject {

protected:

	bool moves[5];
	bool isJumping;

public:

	enum {
		UP = 0,
		RIGHT,
		DOWN,
		LEFT,
		JUMP
	};

   Player() : Player(OB_TYPE) {};
	Player(Model bm, float radius = 505, float theta = 0, float azimuth = 0, float direction = 0);

    ~Player();

	bool getMoving(int);
	void setMoving(int, bool);
	void setJumping(bool b);
	bool getJumping();

	virtual void integrate(float dt) override;
	virtual void collide(float dt, GameObject & target) override;

};

#endif