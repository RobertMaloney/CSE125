#include "MenuState.h"
#include "..\GameClient.h"
#include "GameSound.h"


bool MenuState::replay_flag = false;
bool MenuState::pause_flag = false;
bool MenuState::submit = false;

MenuState::MenuState()
{
	
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
	music_select = 0;
	gameclient = gc;

	//play menu music
	GameSound::menubgm->play();

	//this->input = &InputHandler::getInstance();
}


/* cleanup():
 * This function should free memory used by the menu state.
 */
void MenuState::cleanup()
{
	//stop menu music
	GameSound::menubgm->stop();
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
	Json::Value config;
	Json::Reader reader;
	ifstream inStream;
	inStream.open("../client/config_client.json");

	if (!reader.parse(inStream, config, true)) {
		inStream.close();
		cerr << "Problem parsing json config file" << endl;
		throw runtime_error("Problem parsing json config.");
	}
	inStream.close();

	//need to use the client's TCP connection that was already created in init()
	TCPConnection * client_connection = gameclient->connection;
	SocketError err = client_connection->connect(config["ip"].asString(), config["port"].asString());
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
				checkMenu();
				break;
			case (MOVE_BACKWARD) :
				menuDown();
				checkMenu();
				break;
			case (CONFIRM) :
				menuEnter();
				break;
			case (ADD) :
				changeBgm();
				break;
			case (SUB) :
				GameSound::playOuch();
				break;
			default:
				//nothing
				break;
		}
	}
}

void MenuState::menuUp()
{
	//play sound
	GameSound::menumove->play();

	MenuStatus m = GraphicsEngine::getMenuStatus();

	menu_select = menu_select - 1;
	//not sure if mod handles negatives
	if (menu_select < 0) menu_select = MENU_SELECTIONS_NUM - 1;


	// Status for GraphicsEngine to select texture, not related to select state in menu state
    if (m == MenuStatus::MWINREPLAY || m == MenuStatus::MWINQUIT){
		GraphicsEngine::setMenuStatus(MenuStatus::MWINREPLAY);
	}
	else if (m == MenuStatus::MLOSEREPLAY || m == MenuStatus::MLOSEQUIT){
		GraphicsEngine::setMenuStatus(MenuStatus::MLOSEREPLAY);
	}
	else if (m == MenuStatus::MCONTINUE || m == MenuStatus::MPAUSEQUIT){
		GraphicsEngine::setMenuStatus(MenuStatus::MCONTINUE);
	}
	else{
		GraphicsEngine::setMenuStatus(MenuStatus::START);
    }
}


void MenuState::menuDown()
{
	//play sound
	GameSound::menumove->play();

	MenuStatus m = GraphicsEngine::getMenuStatus();

	menu_select = (menu_select + 1) % MENU_SELECTIONS_NUM;


	// Status for GraphicsEngine to select texture, not related to select state in menu state
	if (m == MenuStatus::MWINREPLAY || m == MenuStatus::MWINQUIT){
		GraphicsEngine::setMenuStatus(MenuStatus::MWINQUIT);
	}
	else if (m == MenuStatus::MLOSEREPLAY || m == MenuStatus::MLOSEQUIT){
		GraphicsEngine::setMenuStatus(MenuStatus::MLOSEQUIT);
	}
	else if (m == MenuStatus::MCONTINUE || m == MenuStatus::MPAUSEQUIT){
		GraphicsEngine::setMenuStatus(MenuStatus::MPAUSEQUIT);
	}
	else{
		GraphicsEngine::setMenuStatus(MenuStatus::QUIT);
}
}


void MenuState::menuEnter()
{
	if (!submit){
		//play sound
		GameSound::menuconfirm->play();

		//check menu_select state

		switch (menu_select) {
		case REPLAY:
		case (PLAY) :
			menu_select = 0;

			if (replay_flag){
				replay();
				replay_flag = false;
			}
			else if (pause_flag){

				conti();
				pause_flag = false;
			}
			else{
			    play();
			}
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


void MenuState::changeBgm()
{
	music_select = (music_select + 1) % MENU_BGM_SELECTIONS_NUM;
	switch (music_select) {
	case (BGM1) :
		GameSound::menubgm2->stop();
		GameSound::menubgm->play();
		break;
	case (BGM2) :
		GameSound::menubgm->stop();
		GameSound::menubgm2->play();
		break;
	default :
		break;
	}
}

void MenuState::play()
{
	GraphicsEngine::setMenuStatus(MenuStatus::LOADING);
	GraphicsEngine::DrawAndPollMenu();
	//make new TCPconnection and connect to server
	this->connectToServer();
	//if success (no exceptions) login to server
	this->login();

	//gameclient->inMenu = false;
	GameClient::inMenu = false;
	
	GraphicsEngine::setCursor(GLFW_CURSOR_DISABLED);

	//cleanupmenu
	this->cleanup();

	// start ingame bgm here unfortunately
	GameSound::ingamebgm->play();
}

void MenuState::conti()
{

	GameClient::inMenu = false;
	GraphicsEngine::setCursor(GLFW_CURSOR_DISABLED);
}

void MenuState::replay(){

	Packet p;
	p.writeByte(static_cast<byte>(EventType::REPLAY));
	gameclient->connection->send(p);
	GameClient::inMenu = false;
	GameClient::loadDone = false;
	gameclient->setResetting(true);
	GraphicsEngine::setCursor(GLFW_CURSOR_DISABLED);
	GraphicsEngine::reset();

	InputHandler::handleKey(GLFW_KEY_R, GLFW_PRESS, 0);

	GameSound::ingamebgm->play();
}


void MenuState::quit()
{
	//TODO: clean up for safely close....
	GraphicsEngine::CloseGame();
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
		if (replay_flag){
			std::cout << "check menu" << std::endl;
	//		std::cout << "REPLAY" << std::endl;
		}
		else if (pause_flag){
	//		std::cout << "PAUSE" << std::endl;
		}
		else{
//		    std::cout << "PLAY" << std::endl;
		}
		break;
	case (QUIT) :
	//	std::cout << "QUIT" << std::endl;
		break;
	default:
	//	std::cout << "UNKNOWN MENU SELECTION" << std::endl;
		break;
	}
}