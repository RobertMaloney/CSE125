#include "GameEngine.h"


GameEngine::GameEngine() {
	gstate = &GameState::getInstance();
}


GameEngine::~GameEngine() {
}

void GameEngine::calculatePercent(){
	int total = gstate->getTotal();
	int occupied = 0;
	int max = 0;

	for (auto it = gstate->getPlayers().begin(); it != gstate->getPlayers().end(); ++it) {
		int s = (*it)->getScore();
		occupied = occupied + s;
		int p = s / total;
		//std::cout << (*it)->getId() << ": " << p << endl;
		(*it)->setPercent(p);

		if (s > max){
			max = s;
			gstate->top = (*it);
		}
	}	
	//std::cout << occupied << "  " << total << endl;
	if (occupied == total){
		//Win or lose
		for (auto it = gstate->getPlayers().begin(); it != gstate->getPlayers().end(); ++it) {
			if ((*it) == gstate->top){//win
				(*it)->setStatus(GStatus::WIN);
				std::cout << (*it)->getId() << ": YOU WIN "  << endl;
			}
			else{//lose
				(*it)->setStatus(GStatus::LOSE);
				std::cout << (*it)->getId() << ": YOU LOSE " << endl;
			}
			
		}
		endGame();
	}
}

void GameEngine::endGame(){
	std::cout << "GAME END" << endl;
}

