#include "GameState.h"


//ObjectDB & map = ObjectDB::getInstance();

void GameState::init(){
	//world = new World();
	numOfPlayers = 0;
	//world->init();
	map = &(ObjectDB::getInstance());
}

GameObject* GameState::addPlayer(ObjectId theId, Player* p) {
	GameObject* o = map->add(theId, p);
	players.push_back(p);
	numOfPlayers++;
	return o;
}

GameState & GameState::getInstance(){
	static GameState gstate;
	return gstate;
}
