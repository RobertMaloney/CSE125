#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Location.h"
#include "BlobModel.h"
#include "GameObject.h"

using namespace std;
#include <glm.hpp>

class Player {
	//TODO put it outside
protected:

	int pid;
	int speed;
	int power;
	static int numOfPlayers;
	glm::vec4 orientation;
	BlobModel bm;

public:

	Player() :Player(OB_TYPE) {};
	Player(BlobModel thebm) : Player(thebm, 0, 0, 0, 0) {};
	Player(BlobModel, float, float, float, float);

	virtual ~Player();

	int getSpeed();
	int getPower();
	int getPid();

	glm::vec4 & getOrientation();

	void setSpeed(int newSpeed);
	void setPower(int newPower);

};

#endif