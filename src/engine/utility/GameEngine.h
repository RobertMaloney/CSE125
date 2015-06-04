#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "GameObject.h"
#include "GameState.h"
#include "../physics/PhysicsEngine.h"
#include "NPC.h"

using namespace std;

class GameEngine {
private:
   PhysicsEngine * pe;
   Json::Value configFile;

public:

	GameEngine(PhysicsEngine*);
	~GameEngine();


	void calculatePercent();
	void endGame();
	GameState* gstate;
   void generateResources(Json::Value configFile);
   void generateRandomResources(int);
   void generateClouds(int);
   void generateClusterTree(float, float, float, int);
   void generateRockRing();
   void generatePills(int);
   void generateNPC(int);
};


#endif
