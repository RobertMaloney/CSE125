#ifndef GAMESERVER_H
#define GAMESERVER_H

#include <string>
#include <thread>
#include <iostream>
#include <unordered_map>

#include "TCPConnection.h"
#include "TCPListener.h"

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

    void PrintUpdates(deque<Packet> & updates);

    unsigned int maxConnections;
    ClientId nextCid;
    TCPListener* listener;
    unordered_map<ClientId, TCPConnection*>* clients;
};


#endif