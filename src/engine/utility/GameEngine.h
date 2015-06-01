#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "GameObject.h"
#include "GameState.h"

using namespace std;

class GameEngine {

public:

	GameEngine();
	~GameEngine();


	void calculatePercent();
	void endGame();
	GameState* gstate;
	void generateResources(int, int, int);
   void generateRandomResources(int);
   void generateClouds(int);
   void generateClusterTree(float, float, float, int);
   void generateRockRing();
   void generatePills(int);
};


#endif
