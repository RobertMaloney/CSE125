#include "GameEngine.h"


GameEngine::GameEngine() {
	gstate = &GameState::getInstance();
}


GameEngine::~GameEngine() {
}

void GameEngine::calculatePercent(){
	int total = gstate->getTotal();

	for (auto it = gstate->getPlayers().begin(); it != gstate->getPlayers().end(); ++it) {
		int s = (*it)->getScore();
		int p = s / total;
		std::cout << (*it)->getId() << ": " << p << endl;
		(*it)->setPercent(p);
	}
}


