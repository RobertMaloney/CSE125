#include "GameClient.h"


GameClient::GameClient() 
{
    connection = new TCPConnection();
	inMenu = true;
}


GameClient::~GameClient() {
    if (connection) {
        delete connection;
        connection = nullptr;
    }
}


void GameClient::init() {
	//get socket ready
	Socket::initialize();

	//make first state, menu
	MenuState *newstate = new MenuState();

	//then change to that state (this also initializes the state if it's not initialized)
	this->addState(newstate);
}


void GameClient::run() {
	bool DEBUG = true;
	bool loggedIn = false;

	//deque<Packet> updates; 

	this->init();
	//this->login();
	GraphicsEngine::Initialize();

	while (!GraphicsEngine::Closing()) {

		updateState();

		if (inMenu){
			current_state->draw();
			current_state->handleEvents();
			current_state->update();
			//current_state->draw();
		}
		else{
			this->sendEvents(InputHandler::input);
			this->receiveUpdates();

			//Note: currently, it only updates game objects. Each package only includes the id and position of a game object.
			//Note: This method reads updates, translates update, updates game states (in client) and scene graph (in GraphicsEngine)
			this->updateGameState();
			updates.clear();
			GraphicsEngine::DrawAndPoll();
		}

	}

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
	//this->receiveClientInput();
}

/*void GameClient::receiveClientInput()
{
	clientUpdates = vector<Packet>(InputHandler::clientInput);
	InputHandler::clientInput.clear();
}*/

void GameClient::updateGameState() {
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
		obj = gstate.getObject(objId);

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
			//Update the object in game state
			obj->deserialize(*packet);//For now it only updates obj (pos) in game state
		}

		//Update the object in node (in GraphicsEngine)
		GraphicsEngine::updateObject(obj->getId(), obj->getOrientation(), obj->getAngle(), obj->getHeight(), obj->getVisible());


		if (obj->getId() == this->playerid)
		    this->checkGameStatus(dynamic_cast<Player*>(obj));
	}


	//client specific input updates
	/*if (clientUpdates.size() <= 0) {
		return;
	}

	for (auto packet = updates.begin(); packet != updates.end(); ++packet) {
		if (packet->size() <= 0) {
			continue;
		}*/

		// = packet->readByte();
		//get event
		//if it is zoom in
		//graphicengine;.izooomin
		//else it it zoom out
		//then zoom out
	//}

}

void GameClient::checkGameStatus(Player * p){
	if (p->getStatus() == GStatus::WIN){
		std::cout << "I win. yayyyyy" << endl;

        //Another menu status or leaderboard or whatever thing should happen here : ask player to replay or end the game....
		inMenu = true;
		GraphicsEngine::setMenuStatus(MenuStatus::MWINREPLAY);
	}
	else if (p->getStatus() == GStatus::LOSE){
		std::cout << "I lose :(" << endl;

		inMenu = true;
		GraphicsEngine::setMenuStatus(MenuStatus::MLOSEREPLAY);
	}// else do nothing
}

void GameClient::close() {
	GraphicsEngine::Destroy();
	system("pause");
}

void GameClient::updateState()
{
	//check if there's a state to change to
	if (next_state == nullptr)
		return;
	current_state = next_state;
	next_state = nullptr;
}



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


void GameClient::addState(IMenuState *state)
{
	//NOTE: this order matters
	//state->gameclient = this;
	state->init(this);
	this->next_state = state;
	this->states.push_back(state);
}


void GameClient::removeState()
{
	//handle case when there's only one state?

	this->states.pop_back();
	this->next_state = states.back();
}


void GameClient::changeState(IMenuState *state)
{
	//NOTE: this order matters
	//state->gameclient = this;
	state->init(this);
	this->next_state = state;
	this->states.push_back(state);
}
