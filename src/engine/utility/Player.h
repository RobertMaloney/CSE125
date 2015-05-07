#ifndef PLAYER_H
#define PLAYER_H

#include <string>
//#include "Location.h"
#include "Model.h"
#include "GameObject.h"

using namespace std;

class Player : public GameObject {

protected:

	int speed;
	int power;
	bool moves[4];

public:
	enum {
		UP = 0,
		RIGHT,
		DOWN,
		LEFT
	};

	Player() :Player(OB_TYPE) {};
	Player(Model thebm) : Player(thebm, 505, 0, 0, 0) {};
	Player(Model, float, float, float, float);

    ~Player();

	int getSpeed();
	int getPower();
   int getScore();

	void setSpeed(int newSpeed);
	void setPower(int newPower);

	bool getMoving(int);
	void setMoving(int, bool);
};

#endif