#ifndef GAME_STATE_H
#define GAME_STATE_H


#include <vector>
#include "ObjectDB.h"
#include "Player.h"
#include "IGameState.h"


using namespace std;

class GameState : IGameState 
{
protected:

	ObjectDB * map;
	vector<Player*> players;

public:

	void init();
	void cleanup();

	void handleEvents();
	void update();
	void draw();

	bool addPlayer(ObjectId theId, Player* p);
	bool addObject(ObjectId id, GameObject* o);
	GameObject* getObject(ObjectId id);
	int getNumPlayers();
	
	static GameState & getInstance();

};

#endif