#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include "ObjectDB.h"
#include "Player.h"



using namespace std;

class GameState {

protected:

	ObjectDB * map;
	vector<Player*> players;

public:

	void init();

	bool addPlayer(ObjectId theId, Player* p);
	bool addObject(ObjectId id, GameObject* o);
	int getNumPlayers();
	static GameState & getInstance();

};

#endif