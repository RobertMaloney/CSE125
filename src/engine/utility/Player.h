#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Model.h"
#include "IEatable.h"
#include "Status.h"
#include "../physics/MoveableObject.h"

using namespace std;


class Player : public MoveableObject {

protected:

	bool moves[5];
	bool isJumping;

   int score;
   int percent;
   GStatus status;

public:

	enum {
		UP = 0,
		RIGHT,
		DOWN,
		LEFT,
		JUMP
	};



   Player() : Player(OB_TYPE) {};
	Player(Model bm, float radius = 505, float theta = 0, float azimuth = 0, float direction = 0);

    ~Player();

	bool getMoving(int);
	void setMoving(int, bool);
	void setJumping(bool b);
	bool getJumping();

	int getScore();
	void setScore(int s);

	int getPercent();
	void setPercent(int p);

	GStatus getStatus();
	void setStatus(GStatus s);

	virtual void integrate(float dt);
	virtual void collide(float dt, GameObject & target) override;

	void serialize(Packet & p);
	void deserialize(Packet & p);

};

#endif