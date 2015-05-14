#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "GameObject.h"
#include "GameState.h"

using namespace std;

class GameEngine {

protected:

	GameState * gstate;


public:

	void init(GameState * g);
	void handleColliDetect(ObjectId id);
	static GameEngine & getInstance();

};


#endif
