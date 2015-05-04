#include "GameState.h"

GameState::GameState(){
	world = new World();
	numOfPlayers = 0;
}

void GameState::init(){
	world->init();
}

void GameState::addPlayer(BlobModel bm) {
	Player* ptr = new Player(bm);
	players.push_back(ptr);
	world->insert(*ptr);
	numOfPlayers++;
}

void GameState::addResource(ResourceModel rm) {
   GameObject * gameObj = new GameObject();
}