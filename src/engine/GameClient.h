#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <thread>
#include <iostream>
#include <deque>
#include <vector>

#include "network\TCPConnection.h"
#include "utility\InputHandler.h"
#include "graphics\GraphicsEngine.h"
#include "network\Packet.h"
#include "utility\Player.h"
#include "utility\GameState.h"
#include "utility\IdGenerator.h"

//forward declarations


using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::to_string;
using std::cout;

class GameClient {

public:
	GameState gstate;

    GameClient();
    ~GameClient();

	void init();
	void run();
	void login();

    void receiveUpdates(deque<Packet> & updates);
    void sendEvents(deque<Packet> & events);
	void updateGameState(deque<Packet> & updates);

	void changeState(IGameState *state);

private:

	TCPConnection* connection;

    inline void checkError(SocketError err);
    inline bool shouldTerminate(SocketError err);

	//stack of states
	vector<IGameState *> states;
};
#endif