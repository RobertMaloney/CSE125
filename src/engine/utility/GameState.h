#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include "World.h"
#include "Player.h"

using namespace std;

class GameState {

protected:
	World *world;
	vector<Player*> players;
	int numOfPlayers;

public:
	GameState();
	virtual ~GameState();

	void addPlayer(Player::BlobModel bm);


};

#endif