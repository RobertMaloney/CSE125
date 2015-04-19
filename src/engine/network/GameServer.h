#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <string>
#include <thread>
#include <iostream>
#include <unordered_map>

#include "TCPConnection.h"
#include "TCPListener.h"

using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::to_string;
using std::unordered_map;
using std::make_pair;
using std::pair;
using std::hash;
using std::cout;

typedef int ClientId;

class GameServer {

public:

    GameServer();
    ~GameServer();

    void Initialize();
    void Run();
   

private:

    void AcceptWaitingClient();
    void SendUpdates(deque<Packet> & updates);
    void ReceiveEvents(deque<Packet> & events);
    bool ShouldTerminate(SocketError err);
    void PrintUpdates(deque<Packet> & updates);

    ClientId nextCid;
    TCPListener* listener;
    unsigned int maxConnections;
    unordered_map<ClientId, TCPConnection*>* clients;
};


#endif