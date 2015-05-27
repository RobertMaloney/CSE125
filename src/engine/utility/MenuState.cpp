#include "MenuState.h"
#include "..\GameClient.h"

MenuState::MenuState()
{
	submit = false;
}


MenuState::~MenuState()
{
}


/* init():
 * This function should initilaize all graphics for the menu and other things.
 */
void MenuState::init(GameClient* gc)
{
	std::cout << "ENTERING: MenuState" << std::endl;
	menu_select = 0;
	gameclient = gc;
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

	//ObjectId playerId = p.readUInt();
	//gameclient->playerid = playerId;

	gameclient->gstate.init();

	Player * player = new Player();
	cout << gameclient->playerid << ": " << player->getAngle() << endl;
	player->deserialize(p);
	gameclient->playerid = player->getId();
	cout << gameclient->playerid  << ": " << player->getAngle() << endl;

	//Initializes GraphicsEngine for this client with playerId (i.e. ClientID)
	gameclient->gstate.addPlayer(gameclient->playerid, player);
	//Binds player game object with the player node in Graphics engine
	GraphicsEngine::bindPlayerNode(player);

	std::cout << "logging in id " << gameclient->playerid << std::endl;

	//gstate.addplayer
	//MOVED to gamestate  
	
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
	MenuStatus m = GraphicsEngine::getMenuStatus();

	menu_select = menu_select - 1;
	//not sure if mod handles negatives
	if (menu_select < 0) menu_select = MENU_SELECTIONS_NUM - 1;

    if (m == MenuStatus::MWINREPLAY || m == MenuStatus::MWINQUIT){
		GraphicsEngine::setMenuStatus(MenuStatus::MWINREPLAY);
	}
	else if (m == MenuStatus::MLOSEREPLAY || m == MenuStatus::MLOSEQUIT){
		GraphicsEngine::setMenuStatus(MenuStatus::MLOSEREPLAY);
	}
	else{
		GraphicsEngine::setMenuStatus(MenuStatus::START);
	}
}


void MenuState::menuDown()
{
	MenuStatus m = GraphicsEngine::getMenuStatus();

	menu_select = (menu_select + 1) % MENU_SELECTIONS_NUM;

	if (m == MenuStatus::MWINREPLAY || m == MenuStatus::MWINQUIT){
		GraphicsEngine::setMenuStatus(MenuStatus::MWINQUIT);
	}
	else if (m == MenuStatus::MLOSEREPLAY || m == MenuStatus::MLOSEQUIT){
		GraphicsEngine::setMenuStatus(MenuStatus::MLOSEQUIT);
	}
	else{
		GraphicsEngine::setMenuStatus(MenuStatus::QUIT);
	}
}


void MenuState::menuEnter()
{
	if (!submit){
		//check menu_select state
		switch (menu_select) {
		case (PLAY) :
			menu_select = 0;
			play();
			break;
		case (QUIT) :
			menu_select = 0;
			quit();
			break;
		default:
			break;
		}
		submit = true;
	}
}


void MenuState::play()
{
	//make new TCPconnection and connect to server
	this->connectToServer();
	//if success (no exceptions) login to server
	this->login();

	gameclient->inMenu = false;
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