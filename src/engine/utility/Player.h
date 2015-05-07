#ifndef PLAYER_H
#define PLAYER_H

#include <string>
//#include "Location.h"
#include "Model.h"
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
	Player(Model thebm) : Player(thebm, 505, 0, 0, 0) {};
	Player(Model, float, float, float, float);

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