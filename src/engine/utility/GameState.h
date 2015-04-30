#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
//#include "World.h"
#include "ObjectDB.h"
#include "Player.h"



using namespace std;

class GameState {

protected:
	int numOfPlayers;

public:
	ObjectDB map;
	vector<Player*> players;
	

	void init();

	GameObject* addPlayer(ObjectId theId, Player* p);
	static GameState & getInstance();

};

#endif