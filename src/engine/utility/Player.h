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
	//static int numOfPlayers;

	BlobModel bm;

public:

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
};

#endif