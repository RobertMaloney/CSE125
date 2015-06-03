#include "GameState.h"

GameState::GameState()
{
	resetting = false;
}


GameState::~GameState()
{

}

void GameState::init()
{
	std::cout << "Entering GameState" << std::endl;
	//get db
	map = &ObjectDB::getInstance();
}


//server uses this to intialize
void GameState::initWithServer(Json::Value & config)
{
	//get db
	this->configFile = config;
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
   switch (playerId % 4){
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

bool GameState::isResetting(){
	return this->resetting;
}


void GameState::setResetting(bool b) {
	this->resetting = b;
}

void GameState::reset() {
	for (auto it = map->objects.begin(); it != map->objects.end(); ++it) {
		switch (it->second->getType()) {
		case PLAYER: {
				Player* p = dynamic_cast<Player*>(it->second);
				p->loadConfiguration(configFile["player"]);
				//	p->setScore(0);
			//	p->setStatus(GStatus::PENDING);
			//	p->setPercent(0);					
			}
			break;
		default:
			it->second->setVisible(true);
			break;
		}
	}
}

void GameState::reset(ObjectId clientId) {
	static set<ObjectId> clients;

	if (clients.find(clientId) == clients.end()) {
		clients.insert(clientId);
	}

	if (clients.size() == this->players.size()) {
		this->resetting = true;
		clients.clear();
	}
	std::cout << "Reset : " << clientId << std::endl;
}