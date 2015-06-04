#include "GameClient.h"
#include "utility\Config.h"

bool GameClient::inMenu = true;

GameClient::GameClient() 
{
	resetting = false;
    connection = new TCPConnection();
}


GameClient::~GameClient() {
    if (connection) {
        delete connection;
        connection = nullptr;
    }
}


void GameClient::init() {
	//get sound ready
	GameSound::init();

	//get socket ready
	Socket::initialize();

    mstate = new MenuState();
	mstate->init(this);

	//then change to that state (this also initializes the state if it's not initialized)
	//this->addState(newstate);
}


void GameClient::run() {
	bool DEBUG = true;
	bool loggedIn = false;

	//deque<Packet> updates; 

	// LOAD CONFIG
	if (!Config::parseJson("config_client.json"))
		std::cout << "Errors parsing config\n";

	//init graphics
	GraphicsEngine::Initialize();

	this->init();

	while (!GraphicsEngine::Closing()) {

		//updateState();

		if (inMenu){
			mstate->draw();
			mstate->handleEvents();
			mstate->update();
			//current_state->draw();
			std::cout << "menu " << std::endl;
		}
		else{
			std::cout << "game " << std::endl;
			this->sendEvents(InputHandler::input);
			this->receiveUpdates();

			//Note: currently, it only updates game objects. Each package only includes the id and position of a game object.
			//Note: This method reads updates, translates update, updates game states (in client) and scene graph (in GraphicsEngine)
			this->updateGameState();
			updates.clear();
			GraphicsEngine::DrawAndPoll();
		}

	}

	//stop ingame music here unfortunately
	GameSound::ingamebgm->stop();

	this->close();
}


void GameClient::sendEvents(vector<Packet> & events) 
{
	this->connection->send(events);
	events.clear();
}


void GameClient::receiveUpdates()
{
	this->checkError(this->connection->receive(updates));
	clientonly_updates = vector<Packet>(InputHandler::clientInput);
	InputHandler::clientInput.clear();
	//this->receiveClientInput();
}

/*void GameClient::receiveClientInput()
{
	clientUpdates = vector<Packet>(InputHandler::clientInput);
	InputHandler::clientInput.clear();
}*/

bool GameClient::isResetting() {
	return this->resetting;
}
void GameClient::setResetting(bool b) {
	this->resetting = b;
}

void GameClient::updateGameState() {
	if (updates.size() <= 0 && clientonly_updates.size() <= 0) {
		return;
	}

	ObjectId objId;
	GameObject* obj = nullptr;
	byte clientonly_event;

	//sounds
	for (auto packet = clientonly_updates.begin(); packet != clientonly_updates.end(); ++packet) {
		if (packet->size() <= 0) {
			continue;
		}

		clientonly_event = packet->readByte();
		packet->reset();

		switch (clientonly_event) {
		case MOVE_FORWARD:
			GameSound::blobmove->pause();
			GameSound::blobmove->play();
			break;
		case STOP_FORWARD:
			GameSound::blobmove->pause();
			break;
		case MOVE_BACKWARD:
			GameSound::blobmove->pause();
			GameSound::blobmove->play();
			break;
		case STOP_BACKWARD:
			GameSound::blobmove->pause();
			break;
		case MOVE_LEFT:
			GameSound::blobmove->pause();
			GameSound::blobmove->play();
			break;
		case STOP_LEFT:
			GameSound::blobmove->pause();
			break;
		case MOVE_RIGHT:
			GameSound::blobmove->pause();
			GameSound::blobmove->play();
			break;
		case STOP_RIGHT:
			GameSound::blobmove->pause();
			break;
		case JUMP:
			GameSound::jump->play();
			break;
		default:
			//do nothing
			break;
		}
	}

	//clear client only updates
	clientonly_updates.clear();


	//Note: Loop through all packets(gameobjects for now), identify which object it relates to or if it is a new object
	for (auto packet = updates.begin(); packet != updates.end(); ++packet) {
		if (packet->size() <= 0) {
			continue;
		}

		objId = packet->readUInt();
		packet->reset();
		obj = gstate.getObject(objId);

		//sound purposes
		bool oldeat = false;
		bool oldhit = false; 

		//If this game object is new 
		if (!obj) {
			obj = new GameObject();
		
			if (!gstate.addObject(objId, obj)){ // Adds to game state in client
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
			oldeat = obj->getEat();
			oldhit = obj->getHit();
			//Update the object in game state
			if (obj->getType() == PLAYER && obj->getId() != this->playerid){
				Player p;
				p.deserialize(*packet);
				//cout << "Percent = " << (static_cast<Player*>(obj))->getPercent() << endl; 
				GraphicsEngine::updatePercent(obj->getModel(), p.getPercent());
				obj = &p;
			}
			else {
				obj->deserialize(*packet);//For now it only updates obj (pos) in game state
			}
		}

		//Update the object in node (in GraphicsEngine)
		GraphicsEngine::updateObject(	obj->getId(), 
										obj->getOrientation(), 
										obj->getAngle(), 
										obj->getHeight(),
										obj->getScale(),
										obj->getVisible());

		//play collision sounds
		if (obj->getId() == this->playerid) {
			if (oldeat == false && obj->getEat() == true) {
				GameSound::nom->play();
				GraphicsEngine::spawnPSystem(MatrixNode::quatAngle(obj->getOrientation(), 0.f, obj->getHeight(), 1.f));
			}
			if (oldhit == false && obj->getHit() == true)
				GameSound::playOuch();
		}

	}

	Player* thep = dynamic_cast<Player*>(gstate.getObject(this->playerid));
	GraphicsEngine::updatePercent(thep->getModel(), thep->getPercent());
	this->checkGameStatus(thep);
}

void GameClient::checkGameStatus(Player * p){
	if (p->getStatus() == GStatus::WIN && !this->isResetting()){
		std::cout << "I win. yayyyyy" << endl;
		std::cout << p->getPercent() << endl;
        //Another menu status or leaderboard or whatever thing should happen here : ask player to replay or end the game....
		inMenu = true;
		MenuState::submit = false;
		MenuState::replay_flag = true;
	
		GraphicsEngine::setMenuStatus(MenuStatus::MWINREPLAY);
	}
	else if (p->getStatus() == GStatus::LOSE && !this->isResetting()){
		std::cout << "I lose :(" << endl;

		inMenu = true;
		MenuState::submit = false;
		MenuState::replay_flag = true;
		GraphicsEngine::setMenuStatus(MenuStatus::MLOSEREPLAY);
	}

	if (p->getStatus() == GStatus::PENDING) {
		this->setResetting(false);
	}

}

void GameClient::close() {
	GraphicsEngine::Destroy();
	system("pause");
}

/*void GameClient::updateState()
{
	//check if there's a state to change to
	if (next_state == nullptr)
		return;
	current_state = next_state;
	next_state = nullptr;
}*/



void GameClient::checkError(SocketError err) {
	if (this->shouldTerminate(err)) {
		connection->close();
		throw SocketException("Fatal error while communicating over TCPConnection.");
	}
}


bool GameClient::shouldTerminate(SocketError err)
{
	switch (err) {
	case SE_NOERR:
		return false;
	case SE_WOULDBLOCK:
		return false;
	case SE_NODATA:
		return false;
	default:
		return true;
	}
}


/*void GameClient::addState(IMenuState *state)
{
	//NOTE: this order matters
	//state->gameclient = this;
	state->init(this);
	this->next_state = state;
	this->states.push_back(state);
}*/

/*
void GameClient::removeState()
{
	//handle case when there's only one state?

	this->states.pop_back();
	this->next_state = states.back();
}
*/
/*
void GameClient::changeState(IMenuState *state)
{
	//NOTE: this order matters
	//state->gameclient = this;
	state->init(this);
	this->next_state = state;
	this->states.push_back(state);
}
*/
