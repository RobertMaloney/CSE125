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
	//if player wants to connect to server
	if (true) {
		//make new TCPconnection and connect to server
		this->connectToServer();
		//if success (no exceptions) login to server
		this->login();

		//push state to game state
		GameState * gamestate = new GameState();
		gameclient->addState(gamestate);
	}
}


/* update():
 * This function should be called in the game loop and update graphics and menu logic.
 */
void MenuState::update()
{
	//update cursor or omuse in menu
}


/* draw():
* This function should be called in the game loop and draw or redraw menuy graphics.
*/
void MenuState::draw()
{

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