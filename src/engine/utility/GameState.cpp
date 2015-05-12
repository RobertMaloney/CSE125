#include "GameState.h"


void GameState::init(){
	map = &ObjectDB::getInstance();
}


void GameState::cleanup()
{
}


void GameState::handleEvents()
{
}


void GameState::update()
{
}


void GameState::draw()
{
}


bool GameState::addPlayer(ObjectId theId, Player* p) {
	if (!map->add(theId, p)){
		return false;
	}
	p->setId(theId);
	players.push_back(p);
	return true;
}

bool GameState::addObject(ObjectId id, GameObject* o) {
	if (!map->add(id, o)){
		return false;
	}
	o->setId(id);
	return true;
}

GameObject* GameState::getObject(ObjectId id) {
	return map->get(id);
}

GameState & GameState::getInstance(){
	static GameState gstate;
	return gstate;
}

int GameState::getNumPlayers() {
	return players.size();
}
