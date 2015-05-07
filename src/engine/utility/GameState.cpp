#include "GameState.h"
#include "IdGenerator.h"

//ObjectDB & map = ObjectDB::getInstance();

void GameState::init(){
	//world = new World();
	numOfPlayers = 0;
	//world->init();
	map = &(ObjectDB::getInstance());
	std::cout << "map: " << map << std::endl;
	//generateResources(30);
}

GameObject* GameState::addPlayer(ObjectId theId, Player* p) {
	GameObject* o = map->add(theId, p);
	players.push_back(p);
	numOfPlayers++;
	return o;
}

GameState & GameState::getInstance(){
	static GameState gstate;
	return gstate;
}

GameObject* GameState::addResource(ObjectId theId, Resource * ptr) {
   GameObject * o = map->add(theId, ptr);
   resources.push_back(ptr);
   return o;
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
      ObjectId resourceId = IdGenerator::getInstance().getNextId();
      addResource(resourceId, newRe);
      //radius is always 505
      //randomize resource model?? (maybe we should separate blob model from resource model)
      //randomize other coords
   }
}