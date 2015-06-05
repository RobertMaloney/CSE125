#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Model.h"
#include "Resources.h"
#include "Status.h"
#include "../physics/MoveableObject.h"

using namespace std;


class Player : public MoveableObject {
private:
	

protected:

	float SCORE_SCALE_RATIO;
	float SCORE_MASS_RATIO;
	int MAX_BURP_COUNT;
	int MAX_STOMACH_SIZE;

	bool moves[5];
	bool isJumping;

	int score;
	int percent;
	int stomach;
	int min;
	int sec;
	int burp_count;
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
	Player(Model bm) : Player(bm, 500, 0, 0, 0){};
	Player(Model bm, float radius, float theta, float azimuth, float direction);

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
	GStatus statusFromString(string & string);
	
	virtual void integrate(float dt);
	virtual void collide(float dt, GameObject & target) override;
	virtual void loadConfiguration(Json::Value config);

	void serialize(Packet & p);
	void deserialize(Packet & p);

	int getMin();
	void setMin(int min);

	int getSec();
	void setSec(int sec);
};

#endif