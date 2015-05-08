#include "GameState.h"


void GameState::init(){
	map = &ObjectDB::getInstance();
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

void GameState::updateMovingPlayers() {
	// update positions
	for (auto it = players.begin(); it != players.end(); ++it) {
		if ((*it)->getMoving(Player::UP)) {
			float dir = (*it)->getLoc().w;
			(*it)->getLoc().z += glm::cos(glm::radians(dir));
			(*it)->getLoc().y += glm::sin(glm::radians(dir));
		}
		else if ((*it)->getMoving(Player::RIGHT)) {
			(*it)->getLoc().w -= 1.f;
		}
		else if ((*it)->getMoving(Player::DOWN)) {
			float dir = (*it)->getLoc().w;
			(*it)->getLoc().z -= glm::cos(glm::radians(dir));
			(*it)->getLoc().y -= glm::sin(glm::radians(dir));
		}
		else if ((*it)->getMoving(Player::LEFT)) {
			(*it)->getLoc().w += 1.f;
		}
	}
}