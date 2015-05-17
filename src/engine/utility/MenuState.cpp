#include "MenuState.h"
#include "..\GameClient.h"


MenuState::MenuState()
{
}


MenuState::~MenuState()
{
}


/* init():
 * This function should initilaize all graphics for the menu and other things.
 */
void MenuState::init()
{
	std::cout << "ENTERING: MenuState" << std::endl;
	menu_select = 0;
}


/* cleanup():
 * This function should free memory used by the menu state.
 */
void MenuState::cleanup()
{

}


/* handleEvents():
 * This function should be called in the game loop and handle user input.
 */
void MenuState::handleEvents()
{
	//sync updates to input
	receiveInput();
}


/* update():
 * This function should be called in the game loop and update graphics and menu logic.
 */
void MenuState::update()
{
	//update cursor or mouse in menu
	updateMenuState();

	//clear input
	updates.clear();
}


/* draw():
* This function should be called in the game loop and draw or redraw menuy graphics.
*/
void MenuState::draw()
{
	//NOTE: weird cause this updates the input
	GraphicsEngine::DrawAndPollMenu();
}


void MenuState::connectToServer()
{	
	//need to use the client's TCP connection that was already created in init()
	TCPConnection * client_connection = gameclient->connection;
	SocketError err = client_connection->connect(DEFAULT_SERVER_IP, DEFAULT_SERVER_PORT);
	if (gameclient->shouldTerminate(err)) {
		client_connection->close();
		delete client_connection;
		client_connection = nullptr;
		throw SocketException("Connection Failure.");
	}
	client_connection->setNoDelay(true);
	client_connection->setNonBlocking(true);
}


void MenuState::login() 
{
	//Note: Client receives id from server, creates a copy of player (same id) and adds it into the game state of client
	//Note: TODO: current position of player is default 505,0,0,0, need to get real position from server
	Packet p;
	gameclient->connection->setNonBlocking(false);
	gameclient->connection->receive(p);

	ObjectId playerId = p.readUInt();
	//Player* player = new Player(); MOVE TO GAMESTATE
	gameclient->playerid = playerId;

	std::cout << "logging in id " << playerId << std::endl;

	//gstate.addplayer
	//MOVED to gamestate

	//Initializes GraphicsEngine for this client with playerId (i.e. ClientID)
	//MOVED TO GAMESTATE

	//Binds player game object with the player node in Graphics engine
	//GraphicsEngine::bindPlayerNode(player); MOVES TO GAMESTATE
	gameclient->connection->setNonBlocking(true);
}


void MenuState::updateMenuState() {
	if (updates.size() <= 0) {
		return;
	}

	byte input;

	//Note: Loop through all packets(gameobjects for now), identify which object it relates to or if it is a new object
	for (auto packet = updates.begin(); packet != updates.end(); ++packet) {
		if (packet->size() <= 0) {
			continue;
		}

		input = packet->readByte();
		packet->reset();

		//check event type
		switch (input) {
			case (MOVE_FORWARD) :
				menuUp();
				//debugging
				checkMenu();
				break;
			case (MOVE_BACKWARD) :
				menuDown();
				//debugging
				checkMenu();
				break;
			case (CONFIRM) :
				menuEnter();
				break;
			default:
				//nothing
				break;
		}
	}
}

void MenuState::menuUp()
{
	menu_select = menu_select - 1;
	//not sure if mod handles negatives
	if (menu_select < 0) menu_select = MENU_SELECTIONS_NUM - 1;
}


void MenuState::menuDown()
{
	menu_select = (menu_select + 1) % MENU_SELECTIONS_NUM;
}


void MenuState::menuEnter()
{
	//check menu_select state
	switch (menu_select) {
	case (PLAY) :
		play();
		break;
	case (QUIT) :
		quit();
		break;
	default:
		break;
	}
}


void MenuState::play()
{
	//make new TCPconnection and connect to server
	this->connectToServer();
	//if success (no exceptions) login to server
	this->login();

	//push state to game state
	GameState * gamestate = new GameState();
	gameclient->addState(gamestate);
}


//TODO: implement this
void MenuState::quit()
{

}


void MenuState::receiveInput()
{
	updates = vector<Packet>(InputHandler::input);
	InputHandler::input.clear();
}


void MenuState::checkMenu()
{
	//DEBUGGING: check menu selection
	switch (menu_select) {
	case (PLAY) :
		std::cout << "PLAY" << std::endl;
		break;
	case (QUIT) :
		std::cout << "QUIT" << std::endl;
		break;
	default:
		std::cout << "UNKNOWN MENU SELECTION" << std::endl;
		break;
	}
}