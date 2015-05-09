#include "GameEngine.h"


void GameEngine::init(GameState * g){
	gstate = g;
}

//Fake one using positions
void GameEngine::handleColliDetect(ObjectId theId) {
    
}

GameEngine & GameEngine::getInstance(){
	static GameEngine engine;
	return engine;
}