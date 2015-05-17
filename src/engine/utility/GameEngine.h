#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "GameObject.h"
#include "GameState.h"

using namespace std;

class GameEngine {

public:

	GameEngine();
	~GameEngine();

private:
	void init(GameState * g);
	void calculatePercent();
	GameState* gstate;


};


#endif
