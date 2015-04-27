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

    void Initialize(int maxPlayers);
    void Run();
    void SendUpdates(deque<Packet> & updates);
    void ReceiveEvents(deque<Packet> & events);


private:

    void AcceptWaitingClient();
    void PrintUpdates(deque<Packet> & updates);

    inline bool ShouldTerminate(SocketError err);

    ClientId nextCid;
    TCPListener* listener;
    unsigned int maxConnections;
    unordered_map<ClientId, TCPConnection*>* clients;

};


#endif