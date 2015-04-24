#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include "World.h"
#include "Player.h"
#include "BlobModel.h"

using namespace std;

class GameState {

protected:
	World *world;
	vector<Player*> players;
	int numOfPlayers;

public:
	GameState();
	virtual ~GameState();

	void init();
	void addPlayer(BlobModel bm);


};

#endif