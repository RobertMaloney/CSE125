#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "GameObject.h"
#include "GameState.h"
#include "Resources.h"

using namespace std;

class GameEngine {

public:

	GameEngine();
	~GameEngine();


	void calculatePercent();
	void endGame();
	GameState* gstate;
	void generateResources(int);
   void generateClouds(int);
   void generateClusterTree(float, float, float, int);

};


#endif
