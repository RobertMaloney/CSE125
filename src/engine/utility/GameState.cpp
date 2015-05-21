#include "GameState.h"
#include "..\GameClient.h"


GameState::GameState()
{

}


GameState::~GameState()
{

}

void GameState::init()
{
	std::cout << "Entering GameState" << std::endl;
	//get db
	map = &ObjectDB::getInstance();

	//init player
	Player* player = new Player();
	assert(this->addPlayer(gameclient->playerid, player)); 
	//Binds player game object with the player node in Graphics engine
	GraphicsEngine::bindPlayerNode(player);
}


//server uses this to intialize
void GameState::initWithServer()
{
	//get db
	map = &ObjectDB::getInstance();
}


void GameState::cleanup()
{
}


void GameState::handleEvents()
{
	this->sendEvents(InputHandler::input);
	this->receiveUpdates();
}


void GameState::update()
{
	//update the db
	this->updateGameState();
	
	//clear for fresh updates
	updates.clear();
}


void GameState::draw()
{
	//use grpahics engine to draw
	GraphicsEngine::DrawAndPoll();
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

int GameState::getNumPlayers() {
	return players.size();
}

vector<Player*> & GameState::getPlayers(){
	return players;
}

int GameState::getTotal(){
	return this->total;
}

void GameState::setTotal(int t){
	this->total = t;
}


void GameState::sendEvents(vector<Packet> & events) 
{
	gameclient->connection->send(events);
	events.clear();
}


void GameState::receiveUpdates() 
{
	gameclient->checkError(gameclient->connection->receive(updates));
}

//TODO TODO TODO TODO TODO : this method needs to be put back to client 
void GameState::updateGameState() {
	if (updates.size() <= 0) {
		return;
	}

	ObjectId objId;
	GameObject* obj = nullptr;

	//Note: Loop through all packets(gameobjects for now), identify which object it relates to or if it is a new object
   for (auto packet = updates.begin(); packet != updates.end(); ++packet) {
		if (packet->size() <= 0) {
			continue;
		}

		objId = packet->readUInt();
		packet->reset();
		obj = this->getObject(objId);

      Player * curPlayer = (Player *) getObject(gameclient->playerid);
      string curTime = curPlayer->getTime();

		//If this game object is new 
		if (!obj) {
			obj = new GameObject();

			if (!this->addObject(objId, obj)){ // Adds to game state in client
				delete obj;
				obj = nullptr;
				continue;
			}
			else{
				obj->deserialize(*packet);//deserialize here to get the model
			}

			//Add node in scene graph (in GraphicsEngine) and add object-node mapping (in GraphicsEngine)
			GraphicsEngine::insertObject(obj->getId(), GraphicsEngine::addNode(GraphicsEngine::selectModel(obj->getModel()), obj->getVisible()));
		}
		else {
			//Update the object in game state
			obj->deserialize(*packet);//For now it only updates obj (pos) in game state
		}


      // TODO: Add method to deal with players and scores
      if (objId == gameclient->playerid)
      {
         curPlayer = (Player*)obj;
         string time = curPlayer->getTime();
         // Only print if time has changed
         if (curTime.compare(time) != 0 )
            std::cout << curPlayer->getTime() << endl;
      }

		//Update the object in node (in GraphicsEngine)
		GraphicsEngine::updateObject(obj->getId(), obj->getOrientation(), obj->getAngle(), obj->getHeight(), obj->getVisible());

		if (obj->getType() == PLAYER){
			Player * p = dynamic_cast<Player*>(obj);
			if (p->getStatus() == GStatus::WIN){
				//Client needs to output win
				std::cout << "I win. yayyyyy" << endl;
			}
			else if (p->getStatus() == GStatus::LOSE){
				//Client needs to output lose
				std::cout << "I lose :(" << endl;
			}
		}
	}
}
