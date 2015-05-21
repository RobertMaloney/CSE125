#include "GameClient.h"


GameClient::GameClient() 
{
    connection = new TCPConnection();
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

	//deque<Packet> updates; MOVED TO GAMESTATE

	this->init();
	//this->login();
	GraphicsEngine::Initialize();

	while (!GraphicsEngine::Closing()) {

		updateState();
		current_state->handleEvents();

		current_state->update();

		current_state->draw();
	
		//GraphicsEngine::DrawAndPoll();

		//this->sendEvents(InputHandler::input);
		//this->receiveUpdates(updates);

			//Note: currently, it only updates game objects. Each package only includes the id and position of a game object.
			//Note: This method reads updates, translates update, updates game states (in client) and scene graph (in GraphicsEngine)
		//this->updateGameState(updates);
		//updates.clear();
		}

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


void GameClient::addState(IGameState *state)
{
	//NOTE: this order matters
	state->gameclient = this;
	state->init();
	this->next_state = state;
	this->states.push_back(state);
}


void GameClient::removeState()
{
	//handle case when there's only one state?

	this->states.pop_back();
	this->next_state = states.back();
}


void GameClient::changeState(IGameState *state)
{
	//NOTE: this order matters
	state->gameclient = this;
	state->init();
	this->next_state = state;
	this->states.push_back(state);
}
