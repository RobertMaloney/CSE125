#ifndef PLAYER_H
#define PLAYER_H

#include <string>
//#include "Location.h"
#include "BlobModel.h"
#include "GameObject.h"


using namespace std;


class Player : public GameObject {
	//TODO put it outside
protected:

	//ObjectId pid;
	int speed;
	int power;
	bool moves[4];

	BlobModel bm;

public:
	enum {
		UP = 0,
		RIGHT,
		DOWN,
		LEFT
	};

	Player() :Player(OB_TYPE) {};
	Player(BlobModel thebm) : Player(thebm, 505, 0, 0, 0) {};
	Player(BlobModel, float, float, float, float);

    ~Player();

	int getSpeed();
	int getPower();

	//ObjectId getPid();

	//glm::vec4 & getOrientation();

	void setSpeed(int newSpeed);
	void setPower(int newPower);

	bool getMoving(int);
	void setMoving(int, bool);
};

#endif