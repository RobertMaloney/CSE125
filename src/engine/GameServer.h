#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <cassert>
#include <chrono>
#include <thread>
#include <string>
#include <iostream>
#include <unordered_map>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "utility/Physics.h"
#include "network/PacketHandler.h"
#include "network/TCPConnection.h"
#include "network/TCPListener.h"
#include "utility/ObjectDB.h"
#include "utility/IdGenerator.h"
#include "utility/GameState.h"

using std::to_string;
using std::this_thread::sleep_for;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::unordered_map;
using std::make_pair;
using std::pair;
using std::cout;

const long long TIME_PER_FRAME = 1000.f / 60.f;

class PacketHandler;

class GameServer {

public:

    GameServer();
    ~GameServer();

	void run();
    void initialize(int maxConns);

    void tick();
    void processClientEvents();

private:

    void acceptWaitingClient();
    void printUpdates(deque<Packet> & updates);

    inline bool shouldTerminate(SocketError err);
    
	unsigned int maxConnections;

	IdGenerator * idGen;
	Physics* physics;
	GameState* gameState;
    TCPListener* listener;
    PacketHandler* handler;
	unordered_map<TCPConnection*, ObjectId>* clients;

};


bool GameServer::shouldTerminate(SocketError err) {
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

#endif