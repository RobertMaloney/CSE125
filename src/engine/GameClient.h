#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <thread>
#include <iostream>
#include <deque>
#include <vector>

#include "network\TCPConnection.h"
//#include "utility\InputHandler.h"
#include "graphics\GraphicsEngine.h"
//#include "network\Packet.h"
//#include "utility\Player.h"
#include "utility\MenuState.h"
#include "utility\GameState.h"
#include "utility\IdGenerator.h"


using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::to_string;
using std::cout;

class GameClient {

public:
	GameState gstate;
	ObjectId playerid;

	IGameState * current_state;
	IGameState * next_state;
	TCPConnection* connection;
	bool inMenu;

    GameClient();
    ~GameClient();

	void init();
	void cleanup();

	void run();
	void close();

	//managing states
	void addState(IGameState *state);
	void removeState();
	void changeState(IGameState *state); //this is a pop and push

	void updateState();

	void checkError(SocketError err);
	bool shouldTerminate(SocketError err);

	void sendEvents(vector<Packet> & events);
	void receiveUpdates();
	void updateGameState();
	void checkGameStatus(Player *);

private:
	//stack of states
	vector<IGameState *> states;
	vector<Packet> updates;
};
#endif