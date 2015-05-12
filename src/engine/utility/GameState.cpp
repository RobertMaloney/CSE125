#include "GameState.h"

void GameState::init(){
	map = &ObjectDB::getInstance();
}

bool GameState::addPlayer(ObjectId theId, Player* p) {
	if (!map->add(theId, p)){
		return false;
	}
   p->setModel(selectPlayerModel(theId));
	p->setId(theId);
	players.push_back(p);
	return true;
}

// Temp method to select player model
Model GameState::selectPlayerModel(ObjectId playerId){
   switch (playerId % 3){
   case 0:
      return OB_TYPE;
   case 1:
      return GB_TYPE;
   case 2:
      return PB_TYPE;
   default:
      return BB_TYPE;
   }
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

bool GameState::addResource(ObjectId theId, Resource * ptr) {
   bool r = map->add(theId, ptr);
   resources.push_back(ptr);
   return r;
}

int GameState::getNumPlayers() {
	return players.size();
}
