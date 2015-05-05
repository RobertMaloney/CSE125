#ifndef PLAYER_H
#define PLAYER_H

#include <string>
//#include "Location.h"
#include "ResourceModel.h"
#include "GameObject.h"


using namespace std;


class Player : public GameObject {
	//TODO put it outside
protected:

	//ObjectId pid;
	int speed;
	int power;
   int score;
	//static int numOfPlayers;

public:

	Player() :Player(OB_TYPE) {};
	Player(ResourceModel thebm) : Player(thebm, 505, 0, 0, 0) {};
	Player(ResourceModel, float, float, float, float);

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