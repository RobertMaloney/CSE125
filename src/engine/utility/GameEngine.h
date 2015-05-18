#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "GameObject.h"
#include "GameState.h"

using namespace std;

class GameEngine {

public:

	GameEngine();
	~GameEngine();

	void init(GameState * g);

private:
	void calculatePercent();
	GameState* gstate;

};


#endif
