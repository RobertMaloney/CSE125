#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <string>
#include <thread>
#include <iostream>
#include <unordered_map>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "network\PacketHandler.h"
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

static ObjectId nextObjId;

class GameServer {

public:

    GameServer();
    ~GameServer();

    void Initialize(int maxConns);
    void Run();

  //  void SendUpdates(deque<Packet> & updates);
  //  void ReceiveEvents(deque<Packet> & events);

private:

    void AcceptWaitingClient();
    void PrintUpdates(deque<Packet> & updates);
	void ParsePlayer(deque<Packet> & in, deque<Packet> & out);

    inline bool ShouldTerminate(SocketError err);
    
	TCPListener* listener;
	unsigned int maxConnections;
	unordered_map<TCPConnection*, ObjectId>* clients;

};


bool GameServer::ShouldTerminate(SocketError err) {
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