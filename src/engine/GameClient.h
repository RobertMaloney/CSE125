#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <thread>
#include <iostream>
#include <deque>
#include <vector>
#include <chrono>

#include "network\TCPConnection.h"
//#include "utility\InputHandler.h"
#include "graphics\GraphicsEngine.h"
//#include "network\Packet.h"
//#include "utility\Player.h"
#include "utility\MenuState.h"
#include "utility\GameState.h"
#include "utility\IdGenerator.h"
#include "utility\GameSound.h"


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::duration;

using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::to_string;
using std::cout;

class GameClient {

public:
	GameState gstate;
	ObjectId playerid;

	IMenuState * mstate;
	TCPConnection* connection;
	static bool inMenu;

    GameClient();
    ~GameClient();

	void init();
	void cleanup();

	void run();
	void close();

	//managing states
	void addState(IMenuState *state);
	void removeState();
	void changeState(IMenuState *state); //this is a pop and push

	//void updateState();

	void checkError(SocketError err);
	bool shouldTerminate(SocketError err);

	void sendEvents(vector<Packet> & events);
	void receiveUpdates();
	void updateGameState();
	void checkGameStatus(Player *);
	//void receiveClientInput();

private:
	//stack of states
	vector<IMenuState *> states;
	vector<Packet> updates;
	vector<Packet> clientonly_updates;
	//vector<Packet> clientUpdates;
};
#endif