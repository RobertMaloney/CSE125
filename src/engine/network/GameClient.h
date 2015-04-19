#ifndef GAME_CLIENT_H
#define GAME_CLIENT_H

#include <thread>
#include <iostream>

#include "TCPConnection.h"

using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using std::to_string;
using std::cout;

class GameClient {

public:

    GameClient();
    ~GameClient();

    void Initialize();
    void ReceiveUpdates(deque<Packet> & updates);
    void SendEvents(deque<Packet> & events);

private:

    bool ShouldTerminate(SocketError err);
    void PrintUpdates(deque<Packet> & updates);

    TCPConnection* connection;

};


#endif