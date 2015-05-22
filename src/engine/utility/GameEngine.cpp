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


void GameEngine::generateResources(int num) {
	int total = 0;
	for (int i = 0; i < num; i++)
	{
		float radius = 505;
		float theta = (float)(rand() % 180);
		float azimuth = (float)(rand() % 360);
		float direction = (float)(rand() % 360);
		Resource * newRe;

		int pick = rand() % 6;


		//Scores are placeholder, need to handle them differently...
		if (pick == 0){
			newRe = new Tree(30, radius, theta, azimuth, direction);
			total = total + 30;
		}
		else if (pick == 1)
			newRe = new Rock(radius, theta, azimuth, direction);
		else if (pick == 2){
			newRe = new Stump(10, radius, theta, azimuth, direction);
			total = total + 10;
		}
		else if (pick == 3)
			newRe = new Grass(radius, theta, azimuth, direction);
		else if (pick == 4){
			newRe = new Mushroom(25, radius, theta, azimuth, direction);
			total = total + 25;
		}
		else if (pick == 5){
			newRe = new Flower(40, radius, theta, azimuth, direction);
			total = total + 40;
		}

		ObjectId resourceId = IdGenerator::getInstance().createId();
		gstate->addResource(resourceId, newRe);

		//radius is always 505
		//randomize resource model?? (maybe we should separate blob model from resource model)
		//randomize other coords
	}
	gstate->setTotal(total);
}

