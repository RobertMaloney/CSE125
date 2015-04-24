#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Location.h"
#include "BlobModel.h"
#include "GameObject.h"

using namespace std;

class Player: public GameObject {
	//TODO put it outside
protected:

	ObjectId pid;
	int speed;
	int power;
	static int numOfPlayers;

	BlobModel bm;

public:

	Player() :Player(OB_TYPE) {};
	Player(BlobModel thebm) :Player(thebm, 0, 0, 0) {};
	Player(BlobModel thebm, int nx, int ny, int nz);

	virtual ~Player();

	int getSpeed();
	int getPower();

	ObjectId getPid();

	void setSpeed(int newSpeed);
	void setPower(int newPower);

};

#endif