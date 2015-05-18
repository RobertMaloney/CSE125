#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Model.h"
#include "MoveableObject.h"
#include "GameObject.h"
#include "Eatable.h"

using namespace std;

class Player : public MoveableObject {

protected:

	bool moves[4];

   int score;

public:

	enum {
		UP = 0,
		RIGHT,
		DOWN,
		LEFT
	};

   Player() : Player(OB_TYPE) {};
	Player(Model bm, float radius = 505, float theta = 0, float azimuth = 0, float direction = 0);

    ~Player();

	bool getMoving(int);
	void setMoving(int, bool);

	virtual void move(float dt) override;
	virtual void collide(float dt, GameObject & target) override;

};

#endif