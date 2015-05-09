#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "BlobModel.h"
#include "GameObject.h"

using namespace std;

class Player : public GameObject {

protected:

	bool moves[4];
	const float PLAYER_ACCELERATION = .001;
	const float FRICTION = .005;
	float velocity;
 
	BlobModel bm;

public:

	enum {
		UP = 0,
		RIGHT,
		DOWN,
		LEFT
	};

	Player() : Player(OB_TYPE) {};
	Player(BlobModel bm, float radius = 505, float theta = 0, float azimuth = 0, float direction = 0);

    ~Player();


	bool getMoving(int);
	void setMoving(int, bool);


	void update(float dt);

};

#endif