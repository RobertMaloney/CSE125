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
    void PrintUpdates(deque<Packet> & updates);

private:

    inline void CheckError(SocketError err);
    inline bool ShouldTerminate(SocketError err);
    
    TCPConnection* connection;

};


void GameClient::CheckError(SocketError err) {
    if (this->ShouldTerminate(err)) {
        connection->Close();
        throw SocketException("Fatal error while communicating over TCPConnection.");
    }
}


bool GameClient::ShouldTerminate(SocketError err) {
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