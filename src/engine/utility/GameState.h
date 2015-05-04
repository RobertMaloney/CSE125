#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include "World.h"
#include "Player.h"
#include "Resource.h"
#include "ResourceModel.h"

using namespace std;

class GameState {

protected:
	World *world;
	vector<Player*> players;
   vector<Resource *> resources;
	int numOfPlayers;

public:
	GameState();
	virtual ~GameState();

	void init();
	void addPlayer(ResourceModel bm);
   void addResource(ResourceModel rm);
   void addResource(Resource * ptr);

   void generateResources(int num);
};

#endif