#include "GameState.h"


//ObjectDB & map = ObjectDB::getInstance();

void GameState::init(){
	//world = new World();
	numOfPlayers = 0;
	//world->init();
	map = &(ObjectDB::getInstance());
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

void GameState::addResource(Resource * ptr) {
   resources.push_back(ptr);
   world->insert(ptr);
}

void GameState::addResource(ResourceModel rm) {
   GameObject * gameObj = new GameObject();
}

void GameState::generateResources(int num) {
   for (int i = 0; i < num; i++)
   {
      float radius = 505;
      float theta = (float)rand() / (float)RAND_MAX;
      float azimuth = (float)rand() / (float)RAND_MAX;
      float direction = (float)(rand() % 360);
      ResourceModel model = TREE;

      int pick = num % 5;
      if (pick == 1)
         model = ROCK;
      else if (pick == 2)
         model = STUMP;
      else if (pick == 3)
         model = GRASS;
      else if (pick == 4)
         model = MUSHROOM;

      Resource * newRe = new Resource(model, 5, radius, theta, azimuth, direction);
      addResource(newRe);
      //radius is always 505
      //randomize resource model?? (maybe we should separate blob model from resource model)
      //randomize other coords
   }
}