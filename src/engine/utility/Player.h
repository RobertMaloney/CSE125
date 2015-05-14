#ifndef PLAYER_H
#define PLAYER_H

#include <string>
//#include "Location.h"
#include "Model.h"
#include "GameObject.h"

using namespace std;

class Player : public GameObject {

protected:

	const float PLAYER_ACCELERATION = .0005;
	const float FRICTION = .005;

	bool moves[4];
	float velocity;

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

	virtual void update(float dt) override;
	virtual void collide(float dt, const GameObject & target) override;

};

#endif