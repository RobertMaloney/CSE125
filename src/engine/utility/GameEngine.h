#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "GameObject.h"
#include "GameState.h"
//#include "Status.h"

using namespace std;

class GameEngine {

public:

	GameEngine();
	~GameEngine();


	void calculatePercent();
   void determineWinner();
	void endGame();
	GameState* gstate;

};


#endif
