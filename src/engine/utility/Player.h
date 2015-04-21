#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Location.h"

using namespace std;

class Player {

protected:

	int pid;
	string id;
	int speed;
	int power;
	Location loc;
	static int numOfPlayers;


	BlobModel bm;

public:


	Player() :Player(OB) {};
	Player(BlobModel thebm);
public:
	Player();
	virtual ~Player();

	int getSpeed();
	int getPower();
	int getPid();
    string getId();

	int getX();
	int getY();
	int getZ();

	Location getLoc();

	void setSpeed(int newSpeed);
	void setPower(int newPower);

	void setX(int newX);
	void setY(int newY);
	void setZ(int newZ);

	void setLoc(Location newLoc);

};

#endif