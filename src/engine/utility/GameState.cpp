#include "GameState.h"


void GameState::init(){
	map = &ObjectDB::getInstance();
}

bool GameState::addPlayer(ObjectId theId, Player* p) {
	if (!map->add(theId, p)){
		return false;
	}
	players.push_back(p);
	return true;
}

bool GameState::addObject(ObjectId id, GameObject* o) {
	if (!map->add(id, o)){
		return false;
	}
	return true;
}

GameState & GameState::getInstance(){
	static GameState gstate;
	return gstate;
}

int GameState::getNumPlayers() {
	return players.size();
}