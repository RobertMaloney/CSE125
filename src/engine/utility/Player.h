#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Model.h"
#include "MoveableObject.h"
#include "GameObject.h"
#include "IEatable.h"

using namespace std;

class Player : public MoveableObject {

protected:

   bool moves[4];

   int score;
   int percent;

public:

	enum {
		UP = 0,
		RIGHT,
		DOWN,
		LEFT
	};

    Player() : Player(OB_TYPE) {};
	Player(Model bm, float radius = 505, float theta = 0, float azimuth = 0, float direction = 0);

    ~Player();

	bool getMoving(int);
	void setMoving(int, bool);

	int getScore();
	void setScore(int s);

	int getPercent();
	void setPercent(int p);

	virtual void move(float dt) override;
	virtual void collide(float dt, GameObject & target) override;

	void serialize(Packet & p);
	void deserialize(Packet & p);

};

#endif