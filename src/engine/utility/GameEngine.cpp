#include "GameEngine.h"


GameEngine::GameEngine(PhysicsEngine * pe) {
	gstate = &GameState::getInstance();
    this->pe = pe;
}


GameEngine::~GameEngine() {
}

void GameEngine::calculatePercent(Timer* t){
	int total = gstate->getTotal();
	int occupied = 0;
	int max = 0;

	for (auto it = gstate->getPlayers().begin(); it != gstate->getPlayers().end(); ++it) {
		int s = (*it)->getScore();
		occupied = occupied + s;
		int p = int(float(s) / total * 100.0);
		//std::cout << (*it)->getId() << ": " << p << endl;
		(*it)->setPercent(p);
		//std::cout << (*it)->getId()  << (*it)->getPercent() << endl;
		//cout << "score = " << s << " total = " << total << " p = " << p << endl;
		if (s > max){
			max = s;
			gstate->top = (*it);
		}
	}	
	//cout << "time: " << t->getMinRemaining() << "  " << t->getSecRemaining() << endl;
	//std::cout << occupied << "  " << total << endl;
	if (occupied >= total || (t->getMinRemaining() == 0 && t->getSecRemaining() == 0) ){
		//Win or lose
		for (auto it = gstate->getPlayers().begin(); it != gstate->getPlayers().end(); ++it) {
			if ((*it) == gstate->top && (*it)->getPercent()!= 0){//win
				(*it)->setStatus(GStatus::WIN);
				//std::cout << (*it)->getId() << ": YOU WIN "  << endl;
			}
			else{//lose
				(*it)->setStatus(GStatus::LOSE);
				//std::cout << (*it)->getId() << ": YOU LOSE " << endl;
			}
			
		}
		endGame();
	}
}

void GameEngine::endGame(){
	//std::cout << "GAME END" << endl;
}

void GameEngine::generateResources(Json::Value configFile)
{
   this->configFile = configFile;
   generateRandomResources(configFile["num resources"].asInt());
   //generateRandomResources(1);
   generateClouds( configFile["num clouds"].asInt());
   generatePills(configFile["num pills"].asInt());
  // //generateClusterTree(505, 10, 10, 200);
   ////generateRockRing();
   generateNPC(configFile["num npcs"].asInt());
}

void GameEngine::generateRandomResources(int num) {
	srand(time(NULL));
	int total = 0;
	for (int i = 0; i < num; i++)
	{
		float radius = 505;
		float theta = (float)(rand() % 180);
		float azimuth = (float)(rand() % 360);
		float direction = (float)(rand() % 360);
      Resource * newRe;


		int pick = rand() % 5;


		// tree      xy 6.f z 16.f
		// trunk     xyz      4.f
		// rock      xy 8.f z 4.f
		// mushroom  xy 2.f z 4.f
		// flower    xy 2.f z 1.5f
		//Scores are placeholder, need to handle them differently...
		if (pick == 0){
         newRe = new Tree(30, 500, theta, azimuth, direction);
         float floor = 1.0, ceiling = 2.0, range = (ceiling - floor);
         float scale = floor + float((range * rand()) / (RAND_MAX + 1.0));
         newRe->setScale(scale);
			newRe->setModelRadius(3.f);
			newRe->setModelHeight(17.f);
			total = total + 30;
		}
		else if (pick == 1) {
			newRe = new Rock(0,radius, theta, azimuth, direction);
			newRe->setModelRadius(2.f);
			newRe->setModelHeight(4.5f);
		}
		else if (pick == 2){
			newRe = new Stump(10, radius, theta, azimuth, direction);
			newRe->setModelRadius(2.f);
			newRe->setModelHeight(4.f);
			total = total + 10;
		}
		else if (pick == 3){
			newRe = new Mushroom(25, radius, theta, azimuth, direction);
			newRe->setModelRadius(1.f);
			newRe->setModelHeight(4.f);
			total = total + 25;
		}
		else if (pick == 4){
			newRe = new Flower(40, radius, theta, azimuth, direction);
			newRe->setModelRadius(1.f);
			newRe->setModelHeight(1.5f);
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

void GameEngine::generateClouds(int num) {
   for (int i = 0; i < num; i++)
   {
      float floor = 600, ceiling = 700, range = (ceiling - floor);
      float radius = floor + float((range * rand()) / (RAND_MAX + 1.0));

      float theta = (float)(rand() % 360);
      float azimuth = (float)(rand() % 360);
      float direction = (float)(0);
      Resource * newRe = new Cloud(15.f, radius, theta, azimuth, direction);

      ObjectId resourceId = IdGenerator::getInstance().createId();
      gstate->addResource(resourceId, newRe);
   }
}

void GameEngine::generateClusterTree(float radius, float theta, float azimuth, int num)
{
   float adist = .3f;
   float tdist = .3f;
   int total = gstate->getTotal();
   for (int i = 0; i < num; i++)
   {
      float floor = theta - tdist, ceiling = theta + tdist, range = (ceiling - floor);
      float theta = floor + float((range * rand()) / (RAND_MAX + 1.0));

      floor = azimuth - adist, ceiling = azimuth + adist, range = (ceiling - floor);
      float azimuth = floor + float((range * rand()) / (RAND_MAX + 1.0));

      float direction = (float)(rand() % 360);
      Resource * newRe;

      int pick = rand() % 100;

      // tree      xy 6.f z 16.f
      // trunk     xyz      4.f
      // rock      xy 8.f z 4.f
      // mushroom  xy 2.f z 4.f
      // flower    xy 2.f z 1.5f
      //Scores are placeholder, need to handle them differently...
      if (pick >= 0 && pick < 60){
         newRe = new Tree(30, 500, theta, azimuth, direction);
         float floor = 1.0, ceiling = 2.0, range = (ceiling - floor);
         float scale = floor + float((range * rand()) / (RAND_MAX + 1.0));
         newRe->setScale(scale);
         newRe->setModelRadius(3.f);
         newRe->setModelHeight(17.f);
         total = total + 30;
      }
      else if (pick >= 60 && pick < 70) {
         newRe = new Rock(0,radius, theta, azimuth, direction);
         newRe->setModelRadius(2.f);
         newRe->setModelHeight(4.5f);
      }
      else if (pick >= 70 && pick < 80){
         newRe = new Stump(10, radius, theta, azimuth, direction);
         newRe->setModelRadius(2.f);
         newRe->setModelHeight(4.f);
         total = total + 10;
      }
      else if (pick >= 80 && pick < 90){
         newRe = new Mushroom(25, radius, theta, azimuth, direction);
         newRe->setModelRadius(1.f);
         newRe->setModelHeight(4.f);
         total = total + 25;
      }
      else if (pick >= 90){
         newRe = new Flower(40, radius, theta, azimuth, direction);
         newRe->setModelRadius(1.f);
         newRe->setModelHeight(1.5f);
         total = total + 40;
      }

      //cout << azimuth << " " << theta << " " << direction;
      ObjectId resourceId = IdGenerator::getInstance().createId();
      gstate->addResource(resourceId, newRe);
   }
   gstate->setTotal(gstate->getTotal() + total);
}

void GameEngine::generateRockRing()
{
   float adist = .2f;
   float tdist = 1.f;
   float theta = 0.f;
   float azimuth = 5.f;
   float radius = 505;
   int num = 10;

   for (int i = 0; i < num; i++)
   {
      //azimuth = fmod(azimuth + adist, 360);
      float floor = azimuth - adist, ceiling = azimuth + adist, range = (ceiling - floor);
      azimuth = floor + float((range * rand()) / (RAND_MAX + 1.0));

      for (int j = 0; j < num; j++)
      {
         //float floor = theta - tdist, ceiling = theta + tdist, range = (ceiling - floor);
         //theta = floor + float((range * rand()) / (RAND_MAX + 1.0));

         theta = fmod(theta + tdist, 360);

         //cout << azimuth << " " << theta << endl;
         //float floor = azimuth - adist, ceiling = azimuth + adist, range = (ceiling - floor);
         //azimuth = floor + float((range * rand()) / (RAND_MAX + 1.0));

         float direction = (float)(0);
         Resource * newRe = new Rock(0,radius, theta, azimuth, direction);

         ObjectId resourceId = IdGenerator::getInstance().createId();
         gstate->addResource(resourceId, newRe);
      }
   }
}

void GameEngine::generatePills(int num) {
   for (int i = 0; i < num; i++)
   {
      float radius = 505;

      float theta = (float)(rand() % 360);
      float azimuth = (float)(rand() % 360);
      float direction = (float)(0);
      Resource * newRe = new Pill(radius, theta, azimuth, direction);

      ObjectId resourceId = IdGenerator::getInstance().createId();
      gstate->addResource(resourceId, newRe);
   }
}

void GameEngine::generateNPC(int num) {
   int total = 0;
   for (int i = 0; i < num; i++)
   {
      float radius = 500;

      float theta = (float)(rand() % 360);
      float azimuth = (float)(rand() % 360);
      float direction = (float)(rand() % 360);

      int pick = int(rand() % 2);
      MoveableObject * newNPC;

      if (pick == 0)
      {
         newNPC = new Bunny(radius, theta, azimuth, direction);
         newNPC->addVelocity(newNPC->rotateInXYPlane(newNPC->getVelocity(), direction));
         newNPC->loadConfiguration(configFile["bunny"]);
         pe->registerInteraction(newNPC, DRAG | GRAVITY);
      }
      else if (pick == 1)
      {
         float floor = 600, ceiling = 700, range = (ceiling - floor);
         float bradius = floor + float((range * rand()) / (RAND_MAX + 1.0));

         newNPC = new Bird(bradius, theta, azimuth, direction);
         newNPC->addVelocity(newNPC->rotateInXYPlane(newNPC->getVelocity(), direction));
         newNPC->loadConfiguration(configFile["bird"]);
         pe->registerInteraction(newNPC, 0);
      }

      total = total + ((NPC*)newNPC)->getPoints();
      ObjectId resourceId = IdGenerator::getInstance().createId();
      gstate->addObject(resourceId, newNPC);
   }
   gstate->setTotal(gstate->getTotal() + total);
}

void GameEngine::updatePlayerTime(int min, int sec) {
	for (auto it = gstate->getPlayers().begin(); it != gstate->getPlayers().end(); ++it) {
		(*it)->setMin(min);
		(*it)->setSec(sec);
	}
}