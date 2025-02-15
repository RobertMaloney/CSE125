#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <cassert>
#include <chrono>
#include <thread>
#include <mutex>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <condition_variable>

#include "json/json.h"
#include "physics/PhysicsEngine.h"
#include "physics/DragGenerator.h"
#include "network/PacketHandler.h"
#include "network/TCPConnection.h"
#include "network/TCPListener.h"
#include "utility/ObjectDB.h"
#include "utility/IdGenerator.h"
#include "utility/GameState.h"
#include "utility/GameEngine.h"
#include "utility/Timer.h"

using std::ifstream;
using std::to_string;
using std::this_thread::sleep_for;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::unordered_map;
using std::make_pair;
using std::pair;
using std::cout;
using std::mutex;
using std::condition_variable;

class PacketHandler;

class GameServer {

public:

    GameServer();
    ~GameServer();

	static bool loadDone;

	void run();
	void stop();
	void reset();
    void initialize();
	void loadConfiguration(Json::Value & values);

    void tick();
	void getUpdates(vector<Packet> & updates);
    void processClientEvents();

private:

    void acceptWaitingClient();
    void printUpdates(deque<Packet> & updates);
    inline bool shouldTerminate(SocketError err);
    
	unsigned int maxConnections;

	long long TIME_PER_FRAME;
	float PHYSICS_DT;

	
	bool running;
	bool gameStarted;


	mutex serverLock;
	condition_variable serverCV;
	Json::Value configFile;
	IdGenerator * idGen;
	PhysicsEngine* physics;
	GameEngine* engine;
	GameState* gameState;
    TCPListener* listener;
    PacketHandler* handler;
	unordered_map<TCPConnection*, ObjectId>* clients;
	Timer * timer;
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