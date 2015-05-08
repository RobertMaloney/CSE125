#include "GameState.h"
#include "IdGenerator.h"


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

void GameState::generateResources(int num) {
   for (int i = 0; i < num; i++)
   {
      float radius = 505;
      float theta = (float)(rand() % 180);
      float azimuth = (float)(rand() % 360);
      float direction = (float)(rand() % 360);
      Model model = TREE;

      int pick = rand() % 6;
      if (pick == 1)
         model = ROCK;
      else if (pick == 2)
         model = STUMP;
      else if (pick == 3)
         model = GRASS;
      else if (pick == 4)
         model = MUSHROOM;
      else if (pick == 5)
         model = FLOWER;

      cout << theta << " " << azimuth << " " << direction;
      Resource * newRe = new Resource(model, 5, radius, theta, azimuth, direction);
      ObjectId resourceId = IdGenerator::getInstance().createId();
      addResource(resourceId, newRe);
      //radius is always 505
      //randomize resource model?? (maybe we should separate blob model from resource model)
      //randomize other coords
   }
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