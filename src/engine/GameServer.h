#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <string>
#include <thread>
#include <iostream>
#include <unordered_map>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

//#include "network\PacketHandler.h"
#include "network\TCPConnection.h"
#include "network\TCPListener.h"

using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::to_string;
using std::unordered_map;
using std::make_pair;
using std::pair;
using std::hash;
using std::cout;

typedef unsigned int ObjectId;
static ObjectId nextObjId;

class GameServer {

public:

    GameServer();
    ~GameServer();

    void initialize(int maxConns);
    void run();

  //  void SendUpdates(deque<Packet> & updates);
  //  void ReceiveEvents(deque<Packet> & events);

private:

    void acceptWaitingClient();
    void printUpdates(deque<Packet> & updates);
	void parsePlayer(deque<Packet> & in, deque<Packet> & out);

    inline bool shouldTerminate(SocketError err);
    
	TCPListener* listener;
	unsigned int maxConnections;
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