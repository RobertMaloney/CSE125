#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <thread>
#include <iostream>
#include <deque>

#include "network\TCPConnection.h"
#include "graphics\GraphicsEngine.h"
#include "network\Packet.h"


using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::to_string;
using std::cout;

class GameClient {

public:

	static deque<Packet> input;

    GameClient();
    ~GameClient();

	void run();
    void initialize();

    void receiveUpdates(deque<Packet> & updates);
    void sendEvents(deque<Packet> & events);
    void printUpdates(deque<Packet> & updates);

private:

	TCPConnection* connection;

    inline void checkError(SocketError err);
    inline bool shouldTerminate(SocketError err);
};
#endif