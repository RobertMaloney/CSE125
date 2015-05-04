#ifndef PLAYER_H
#define PLAYER_H

#include <string>
//#include "Location.h"
#include "BlobModel.h"
#include "GameObject.h"


using namespace std;


class Player: public GameObject {
	//TODO put it outside
protected:

	//ObjectId pid;
	int speed;
	int power;
   int score;
	//static int numOfPlayers;

	BlobModel bm;

public:

	Player() :Player(OB_TYPE) {};
	Player(BlobModel thebm) : Player(thebm, 0, 0, 0, 0) {};
	Player(BlobModel, float, float, float, float);

    ~Player();

	int getSpeed();
	int getPower();
   int getScore();

	//ObjectId getPid();

	//glm::vec4 & getOrientation();

	void setSpeed(int newSpeed);
	void setPower(int newPower);
   void setScore(int score);
};

#endif